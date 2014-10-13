<pigbehavior language="JScript">
//imagotrigger@gmail.com 10/14 originally from manuel.pig and mark.pig Microsoft

<script src="include\AutoStartCustomGame.js"/>
<script src="include\common.js"/>
<script>
<![CDATA[

/////////////////////////////////////////////////////////////////////////////
// Settings
//

var GameName = "Perpetual Brawl DM";
var ServerName = "azbuildslave";
var ServerAddr = "191.239.1.217";
var CivSelection = "Iron Coalition,Dreghklar";  //blank for Random
var ShipSelection = "Hvy Interceptor";
var ShootSkill = "0.25";
var TurnSkill = "0.25";

//globals

var MyGarrison;
var MyShip;
var EnemyGarrison;
var CurrentTarget;
var GameController = false;
var RoundCount = 0;

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Logs on to the mission server.
function OnStateNonExistant(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
	// If previous state was PigState_Terminated, the pig is being created
	if (PigState_Terminated == eStatePrevious)
	{
		// Logon to the mission server
		try
		{
			//UpdatesPerSecond = 5;
			Logon();
		}
		catch (e)
		{
			// Logon failed, so shutdown the pig
			Trace("Logon failed: " + e.description + "\n");
			Shutdown();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins or creates a mission.
function OnStateMissionList(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
	// Attempt to join the current pig mission, if any
	try
	{
		Trace("Trying to join "+GameName+"...\n");
		JoinMission(GameName);
		return;
	}
	catch (e)
	{
		Trace("JoinMission("+GameName+") failed: " + e.description + "\n");
	}

	Trace("No JoinMission worked, so CreateMission("+ServerName+","+ServerAddr+");...\n");
	// No missions exist, create one and join it
	var objParams = new ActiveXObject("Pigs.MissionParams");
	objParams.TeamCount = 2;
	objParams.MinTeamPlayers = 1;
	objParams.MaxTeamPlayers = 10;
	objParams.GameName = GameName;
	objParams.CoreName = "Pcore006";
	objParams.MapType = PigMapType_Brawl;
	objParams.TeamKills = 25;
	objParams.KillBonus = 0;
	objParams.Defections = true;
	objParams.Miners = 0;
	objParams.Developments = false;
	objParams.Conquest = 0;
	objParams.Flags = 0;
	objParams.Artifacts = 0;
	objParams.Pods = true;
	objParams.Experimental = true;

	CreateMission(ServerName,ServerAddr,objParams);
	GameController = true;
	AutoStartGame(objParams);
	RoundCount++;
}

//joins a team after creating a mission or restarts it
function OnStateWaitingForMission(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
	if (PigState_CreatingMission == eStatePrevious)
	{
		Trace("Creating JoinTimer()\n");
		CreateTimer(3, "JoinTimer()", -1, "JoinTimer");
	}
	if (PigState_Flying == eStatePrevious)
	{
		Trace("Checking IsMissionOwner()\n");
		GameController = IsMissionOwner();
		if (GameController) {
			RoundCount++;
			CreateTimer(3, "ChatStartGameTimer()", -1, "ChatStartGameTimer");
			CreateTimer(30, "StartGameTimer()", -1, "StartGameTimer");
		}
	}	 
}
function JoinTimer()
{
	Timer.Kill();
	Trace("killed timer, Attempting to JoinTeam\n");
	JoinTeam(CivSelection);
}
function StartGameTimer()
{
	Timer.Kill();
	Trace("killed timer, Attempting to StartGame\n");
	StartGame();
}
function ChatStartGameTimer()
{
	Timer.Kill();
	Trace("killed timer, Attempting to SendChat\n");
	Game.SendChat("Auto-restarting round #"+RoundCount+" in 30 seconds...");
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins a random team from NOAT.
function OnStateTeamList(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
	// Avoid repeated attempts to join a team
	if (PigState_JoiningTeam != eStatePrevious)
	{
		Trace("Attempting to JoinTeam\n");
		JoinTeam(CivSelection);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Launches the pig as soon as it becomes docked.
function OnStateDocked(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
	var objHullTypes = HullTypes;
	var iHull = SelectBestHull(objHullTypes,ShipSelection,"Fighter");
	Ship.BuyHull(objHullTypes(iHull));

	Trace("Launching into space...\n");
	Launch();
	//NYI
	//Launch(ShootSkill,TurnSkill);
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Do the moves!
function OnStateFlying(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);

	//determine my ship and garrison
	var x;
	var shiplist = Ship.Sector.Ships;
	var stationlist = Ship.Sector.Stations;
	var e = new Enumerator (shiplist);
	for(; !e.atEnd(); e.moveNext())
	{
		x = e.item();
		//Host.Trace("ship: " + x.Name + "\n");
		if (x.Name == Name) {
			//Trace("Found myself!\n");
			MyShip = x; //this should equal Ship
		} else {
			if (x.Team != MyShip.Team) {
				//Trace("Found enemy ship!\n");
				CurrentTarget = x;
			}
		}
	}
	var f = new Enumerator (stationlist);
	for(; !f.atEnd(); f.moveNext())
	{
		x = f.item();
		//Host.Trace("station: " + x.Name + " Team: "+x.Team+"\n");
		if (x.Name == "Garrison" && x.Team == MyShip.Team) {
			//Trace("Found home!\n");
			MyGarrison = x;
		}
		if (x.Name == "Garrison" && x.Team != MyShip.Team) {
			//Trace("Found enemy garr!\n");
			EnemyGarrison = x;
		}	    
	}

	Trace("Attacking "+CurrentTarget.Name+"\n");
	Game.SendChat("My initial target is: "+CurrentTarget.Name);
	Attack(CurrentTarget.Name);

	//NYI
	//Start the EnemyScanner() loop to update CurrentTarget
	//Boost
	//Shoot Missiles
	//Drop Mines / ECM
}

function OnShipDamaged(objShip, objModel, fAmount, fLeakage, objV1, objV2)
{

	// if it's not me, return
	if (objShip != Ship)
		return;

	// if the attacker is not a ship, return
	if (objModel.ObjectType != 0) //AGC_Ship
		return;

	// send back a chat message
	if (objModel.Team == Ship.Team) {
		objModel.SendChat("Hey, stop hurting me!");
	}
	else
	{
		//attempt to update CurrentTarget
		//NYI


		//objModel.SendChat("I'm gonna get you!");
	}
}

function OnReceiveChat(strText, objShip)
{   
	if (objShip == Ship)
		return;

	//Trace("OnReceiveChat: "+strText+" from: "+objShip.Name+"\n");
}

//fires for the piglet that got blown up
function OnShipKilled(objModel, fAmount, objV1, objV2)
{
	Trace("OnShipKilled: "+fAmount+" killer: "+objModel.Name+"\n");
}

/////////////////////////////////////////////////////////////////////////////
//
function OnActivate(objDeactivated)
{
	Trace("Piglet Activated, previous was " +
		(objDeactivated ? objDeactivated.BehaviorType.Name : "(none)") + "\n");
}


/////////////////////////////////////////////////////////////////////////////
//
function OnDeactivate(objActivated)
{
	Trace("Piglet Dectivated, next is " +
		(objActivated ? objActivated.BehaviorType.Name : "(none)") + "\n");
}



/////////////////////////////////////////////////////////////////////////////
// Displays state transitions.
//
function DisplayStateTransition(eStatePrevious)
{
	Trace("State changed from " + StateName(eStatePrevious)     // Host.Trace    Host.StateName
		+ " to " + PigStateName + "\n");                                       // Pig.PigStateName
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateLoggingOn(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateLoggingOff(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateCreatingMission(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateJoiningMission(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateQuittingMission(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateJoiningTeam(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateLaunching(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateTeminated(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
}


/////////////////////////////////////////////////////////////////////////////
// Handles the 'mission started' notification.
function OnMissionStarted()
{
	// Output debug text
	Trace("OnMissionStarted()\n");
}



// End of script
/////////////////////////////////////////////////////////////////////////////
]]>
</script>
</pigbehavior>

