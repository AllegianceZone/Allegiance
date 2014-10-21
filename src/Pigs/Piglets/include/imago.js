//imagotrigger@gmail.com 10/14

// Globals
var MyGarrison;
var MyShip;
var EnemyGarrison;
var CurrentTarget;
var IsTargetClose = false;
var ReachedEnemy = false;
var GameController = false;
var HailedForRescue = false;
var RoundCount = 0;
var LastKill = (new Date).getTime();
var LastDeath = (new Date).getTime();
var LastDamage = (new Date).getTime();
var AboutToDie = false;

// genesis
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

// step 2...
function OnStateMissionList(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	try {
		JoinMission(GameName);
		return;
	} catch (e) {
		if (e.description != "No missions exist on the server." && e.description != "Specified game not found or has no positions available") {
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
	objParams.KillBonus = 0; // 0 = no kb    2 = c_stdKB
	objParams.Defections = true;
	objParams.Miners = 0;
	objParams.Developments = false;
	objParams.Conquest = 0;
	objParams.Flags = 0;
	objParams.Artifacts = 0;
	objParams.Pods = false;
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

// step 3...
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
			CreateTimer(33, "StartGameTimer()", -1, "StartGameTimer");
		}
		Game.SendChat("My skillz: "+(ShootSkill * 100)+"%");
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
	GameController = IsMissionOwner();
	for (var it = new Enumerator(Game.Teams); !it.atEnd(); it.moveNext())
	if (!GameController || it.item().Ships.Count < MissionParams.MinTeamPlayers) {
		Game.SendChat("Aborting launch...",1301); //voJustASecSound
		if(GameController && "object" != typeof(Properties("AutoStartGameTimer")) && "object" != typeof(Properties("AutoStartGameDelay")))
			CreateTimer(1.0, "AutoStartGame_Tick()", -1, "AutoStartGameTimer");
		return;
	}
	StartGame();
}
function ChatStartGameTimer() {
	Timer.Kill();
	Trace("killed timer, Attempting to SendChat\n");
	Game.SendChat("Auto-restarting round #"+RoundCount+" in 30 seconds...",1308); //voRematchSound
}

// step 4...
function OnStateTeamList(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	if (PigState_JoiningTeam != eStatePrevious) {
		Trace("Attempting to JoinTeam\n");
		JoinTeam(CivSelection);
	}
}

// step 5...
function OnStateDocked(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	KillTimers();
	IsTargetClose = false;
	ReachedEnemy = false;
	AboutToDie = false;
	HailedForRescue = false;
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

// the end...
function OnStateLoggingOff(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	Shutdown();
}

//
// nonsense below!
//
function OnAlephHit(objAleph) {
	Trace("ooh i touched an aleph? wtf is this for\n");
}

function OnSectorChange(objSectorOld, objSectorNew) {
	Trace("ooh i changed sectors\n");
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