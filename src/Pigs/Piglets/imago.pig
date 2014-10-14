<pigbehavior language="JScript">
//imagotrigger@gmail.com 10/14 imago.pig originally from manuel.pig and mark.pig Microsoft

<script src="include\AutoStartCustomGame.js"/>
<script src="include\common.js"/>
<script>
<![CDATA[

// Settings
var GameName = "Bot Testing";
var ServerName = "Imago-PC";
var ServerAddr = "192.168.2.2";
var KillGoal = 30;

var CivSelection = "Iron Coalition,Dreghklar";  //blank for Random
var ShipSelection = "Hvy Interceptor";
var ShootSkill = 0.05;
var TurnSkill = 0.05;
var GotoSkill = 0.05;
UpdatesPerSecond = 30;

// Globals
var MyGarrison;
var MyShip;
var EnemyGarrison;
var CurrentTarget;
var GameController = false;
var RoundCount = 0;
var LastKill = (new Date).getTime();
var LastDeath = (new Date).getTime();
var LastDamage = (new Date).getTime();

function OnStateNonExistant(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	if (PigState_Terminated == eStatePrevious) {
		try { Logon(); }
		catch (e) {
			Trace("Logon failed: " + e.description + "\n");
			Shutdown();
		}
	}
}

function OnStateMissionList(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	try {
		JoinMission(GameName);
		return;
	}
	catch (e) {
		if (e.description != "No missions exist on the server.") {
			Trace("JoinMission failed: " + e.description + "\n");
			Shutdown();
		}
	}

	Trace("No JoinMission worked, so CreateMission("+ServerName+","+ServerAddr+");...\n");
	var objParams = new ActiveXObject("Pigs.MissionParams");
	objParams.TeamCount = 2;
	objParams.MinTeamPlayers = 1;
	objParams.MaxTeamPlayers = 45;
	objParams.GameName = GameName;
	objParams.CoreName = "Pcore006";
	objParams.MapType = PigMapType_Brawl;
	objParams.TeamKills = KillGoal;
	objParams.KillBonus = 2; //c_stdKB 1= low 0= none
	objParams.Defections = true;
	objParams.Miners = 0;
	objParams.Developments = false;
	objParams.Conquest = 0;
	objParams.Flags = 0;
	objParams.Artifacts = 0;
	objParams.Pods = true;
	objParams.Experimental = true;
	try { CreateMission(ServerName,ServerAddr,objParams); } 
	catch(e) {
		Trace("CreateMission("+ServerName+"...) failed: " + e.description + "\n");
		Shutdown();
	}
	GameController = true;
	AutoStartGame(objParams);
	RoundCount++;
}

function OnStateWaitingForMission(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	if (PigState_CreatingMission == eStatePrevious) {
		Trace("Creating JoinTimer()\n");
		CreateTimer(3, "JoinTimer()", -1, "JoinTimer");
	}
	if (PigState_Flying == eStatePrevious) {
		GameController = IsMissionOwner();
		if (GameController) {
			RoundCount++;
			CreateTimer(3, "ChatStartGameTimer()", -1, "ChatStartGameTimer");
			CreateTimer(30, "StartGameTimer()", -1, "StartGameTimer");
		}
	}	 
}
function JoinTimer() {
	Timer.Kill();
	Trace("killed timer, Attempting to JoinTeam\n");
	JoinTeam(CivSelection);
}
function StartGameTimer() {
	Timer.Kill();
	Trace("killed timer, Attempting to StartGame\n");
	StartGame();
}
function ChatStartGameTimer() {
	Timer.Kill();
	Trace("killed timer, Attempting to SendChat\n");
	Game.SendChat("Auto-restarting round #"+RoundCount+" in 30 seconds...");
}

function OnStateTeamList(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	if (PigState_JoiningTeam != eStatePrevious) {
		Trace("Attempting to JoinTeam\n");
		JoinTeam(CivSelection);
	}
}

function OnStateDocked(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	var objHullTypes = HullTypes;
	var iHull = SelectBestHull(objHullTypes,ShipSelection,"Fighter");
	Ship.BuyHull(objHullTypes(iHull));
	if (eStatePrevious != PigState_WaitingForMission || Game.GameStage == 2) { //AGCGameStage_Started
		Trace("Launching into space...\n");
		SetSkills(ShootSkill,TurnSkill,GotoSkill);
		Launch();
	}
}

function OnMissionStarted() {
	Trace("OnMissionStarted()! launching into space...\n");
	SetSkills(ShootSkill,TurnSkill,GotoSkill);
	Launch();
}

function OnStateFlying(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);

	var x;
	var shiplist = Ship.Sector.Ships;
	var stationlist = Ship.Sector.Stations;
	var e = new Enumerator (shiplist);
	for(; !e.atEnd(); e.moveNext()) {
		x = e.item();
		if (x.Name == Name) {
			MyShip = x; //this should equal Ship
		} else {
			if (x.Team != MyShip.Team) {
				CurrentTarget = x;
			}
		}
	}
	var f = new Enumerator (stationlist);
	for(; !f.atEnd(); f.moveNext()) {
		x = f.item();
		if (x.Name == "Garrison" && x.Team == MyShip.Team) {
			MyGarrison = x;
		}
		if (x.Name == "Garrison" && x.Team != MyShip.Team) {
			EnemyGarrison = x;
		}	    
	}

	if (CurrentTarget) {
		Game.SendChat("My initial target is: "+CurrentTarget.Name);
		Attack(CurrentTarget.Name);
	} else {
		CreateTimer(3, "FindTargetTimer()", -1, "FindTargetTimer");
	}
	//TODO Boost
	//TODO Shoot Missiles
	//TODO Drop Mines / ECM
}
function FindTargetTimer() {
	Trace("Attempting to FindNearestEnemy\n");
	var objShips = Ship.Sector.Ships;
	var iShip = FindNearestEnemy(objShips);
	if (iShip != -1) CurrentTarget = objShips(iShip);
	if (CurrentTarget) {
		Game.SendChat("My new initial target is: "+CurrentTarget.Name);
		Attack(CurrentTarget.Name);
		Timer.Kill();
	}	
}
//NYI loop to update CurrentTarget
function UpdateTargetTimer() {
	//current target check
}
//NYI loop to dock when needed
function RearmTimer() {
	//ammo check
	//fuel check
	//damage check
}

function OnShipDamaged(objShip, objModel, fAmount, fLeakage, objV1, objV2) {
	if (!objShip || !Ship || !objModel)
		return;
	if (objShip.ObjectID != Ship.ObjectID)
		return;
	if (objModel.ObjectType != 0) //AGC_Ship
		return;

	if (objModel.Team != Ship.Team) {
		var tnow = (new Date).getTime();
		var delta =  tnow - LastDamage;
		LastDamage = tnow;		
		if (delta > 2000) {
			var newtarget;
			var objShips = Ship.Sector.Ships;
			var iShip = FindNearestEnemy(objShips);
			if (iShip != -1) newtarget = objShips(iShip);
			if (newtarget && newtarget.ObjectID == objModel.ObjectID && newtarget.ObjectID != CurrentTarget.ObjectID) {
				CurrentTarget = newtarget;
				Game.SendChat("My new pesky target is: "+CurrentTarget.Name);
				Attack(CurrentTarget.Name);		
			}
		}
	}
}

function OnShipKilled(objShip, objModel, fAmount, objV1, objV2) {
	if (!objShip || !Ship || !objModel)
		return;
			
	if (objModel.ObjectID == Ship.ObjectID) {
		var tnow = (new Date).getTime();
		var delta =  tnow - LastKill;
		LastKill = tnow;		
		if (delta > 500)	
			Game.SendChat("Yesss.. pwned you!");
	}
	
	if (objShip.ObjectID == Ship.ObjectID) {
		var bht = objShip.BaseHullType;
		var tnow = (new Date).getTime();
		var delta =  tnow - LastDeath;
		LastDeath = tnow;
		if (delta > 2000) {
			ShootSkill += 0.05;
			TurnSkill += 0.05;
			GotoSkill += 0.05;
			Game.SendChat("Nooo.. skills increased!");
		}
	}
}

function OnStateLoggingOff(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	Shutdown();
}

//
//debugging crap below!
//
function OnAlephHit(objAleph) {   
	Trace("ooh i touched an aleph? wtf is this for\n");
}

function OnSectorChange(objSectorOld, objSectorNew) {   
	Trace("ooh i changed sectors\n");
}

function OnReceiveChat(strText, objShip) {   
	//Trace("OnReceiveChat: "+strText+" from: "+objShip.Name+"\n");
}

function OnActivate(objDeactivated) {
	Trace("Piglet Activated, previous was " + (objDeactivated ? objDeactivated.BehaviorType.Name : "(none)") + "\n");
}

function OnDeactivate(objActivated) {
	Trace("Piglet Dectivated, next is " + (objActivated ? objActivated.BehaviorType.Name : "(none)") + "\n");
}

function OnStateLoggingOn(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
}

function OnStateCreatingMission(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	//NYI timeout timer!
}

function OnStateJoiningMission(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	//NYI timeout timer!
}

function OnStateQuittingMission(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
}

function OnStateJoiningTeam(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
}

function OnStateLaunching(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
}

function OnStateTeminated(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
}

]]>
</script>
</pigbehavior>

