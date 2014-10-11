<pigbehavior language="JScript">

<script src="include\AutoStartCustomGame.js"/>
<script src="include\common.js"/>

<script>
<![CDATA[

/////////////////////////////////////////////////////////////////////////////
// Beginning of script
//

var GameName = "Bot DeathMatch";
var MyGarrison;
var MyShip;
var EnemyGarrison;
var CurrentTarget;

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
  Trace("Trying to join Pig Mision...\n");
    JoinMission(GameName);
    return;
  }
  catch (e)
  {
  }

	Trace("No JoinMission Worked so Creating one...\n");
  // No missions exist, create one and join it
  var objParams = new ActiveXObject("Pigs.MissionParams");
  objParams.TeamCount = 2;
  objParams.MinTeamPlayers = 1;
  objParams.MaxTeamPlayers = 10;
  objParams.GameName = GameName;
  objParams.CoreName = "Pcore006";
  objParams.MapType = PigMapType_Brawl;
  objParams.TeamKills = 5;
  //CreateMission("Imago-PC","192.168.2.2",objParams);
  CreateMission("azbuildslave","191.239.1.217",objParams);

  // Automatically start game when the minimum players per team have joined
  AutoStartGame(objParams);
}


function OnStateWaitingForMission(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
	Trace("OnStateWaitingForMission, previous "+eStatePrevious+"...\n");
 	if (PigState_CreatingMission == eStatePrevious)
 	 {
		CreateTimer(3, "OnJoinTimer()", -1, "JoinTimer");
	 }
}

function OnJoinTimer()
{
	Trace("in OnJoinTimer()\n");
     	Timer.Kill();
     	Trace("killed timer, Attempting to JoinTeam\n");
      	JoinTeam();	
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins a random team.
function OnStateTeamList(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
  // Avoid repeated attempts to join a team
  if (PigState_JoiningTeam != eStatePrevious)
  {
     Trace("Attempting to JoinTeam\n");
      JoinTeam();
  }
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Launches the pig as soon as it becomes docked.
function OnStateDocked(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
    var objHullTypes = HullTypes;
    var iHull = SelectBestHull(objHullTypes,"Hvy Interceptor","Fighter");
    Ship.BuyHull(objHullTypes(iHull));

  // Launch into space
  Trace("Launching....\n");
  Launch();
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
	Defend("Garrison");
	
	//determine my ship and garrison
	var x;
	var shiplist = Ship.Sector.Ships;
	var stationlist = Ship.Sector.Stations;
	var e = new Enumerator (shiplist);
	  for(; !e.atEnd(); e.moveNext())
	  {
	    x = e.item();
	    Host.Trace("ship: " + x.Name + "\n");
	    if (x.Name == Name) {
	    	Trace("Found myself!\n");
	      MyShip = x;
	    } else {
	    	if (x.Team != MyShip.Team) {
	    		Trace("Found enemy ship!\n");
	    		CurrentTarget = x;
	    	}
	    }
	  }
	var f = new Enumerator (stationlist);
	  for(; !f.atEnd(); f.moveNext())
	  {
	    x = f.item();
	    Host.Trace("station: " + x.Name + " Team: "+x.Team+"\n");
	    if (x.Name == "Garrison" && x.Team == MyShip.Team) {
	      Trace("Found home!\n");
	      MyGarrison = x;
	    }
	    if (x.Name == "Garrison" && x.Team != MyShip.Team) {
	      Trace("Found enemy garr!\n");
	      EnemyGarrison = x;
	    }	    
	  }
	  Trace("Attacking "+CurrentTarget.Name+"\n");
	  Game.SendChat("My initial target is: "+CurrentTarget.Name);
	  Attack(CurrentTarget.Name);
}

function OnShipDamaged(objShip, objModel, fAmount, fLeakage, objV1, objV2)
{

  // if it's not me, return
  if (objShip != Ship)
    return;

	//Trace("OnShipDamaged from type: "+objModel.ObjectType+" name: "+objModel.Name+"\n");

  // if the attacker is not a ship, return
  if (objModel.ObjectType != 0) //AGC_Ship
    return;

  // send back a chat message
  if (objModel.Team == Ship.Team) {
    objModel.SendChat("Hey, stop shooting me!");
  }
  else
  {
    objModel.SendChat("I'm gonna get you!");
  }
}

function OnReceiveChat(strText, objShip)
{   
	if (objShip == Ship)
		return;
		
    //Trace("OnReceiveChat: "+strText+" from: "+objShip.Name+"\n");
    
    return false;
}

function OnShipKilled(objModel, fAmount, objV1, objV2)
{
	Trace("OnShipKilled: "+fAmount+" dead: "+objModel.Name+"\n");
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

