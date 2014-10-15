//imagotrigger@gmail.com

var MissionParams;

function AutoStartGame(objParams) {
	if (PigState_WaitingForMission != PigState)
		throw "Cannot call AutoStartGame unless PigState is PigState_WaitingForMission";
	MissionParams = objParams;
	CreateTimer(1.0, "AutoStartGame_Tick()", -1, "AutoStartGameTimer");
}

function AutoStartGame_Tick() {
	for (var it = new Enumerator(Game.Teams); !it.atEnd(); it.moveNext())
	if (it.item().Ships.Count < MissionParams.MinTeamPlayers)
	  return;
	Timer.Kill();
	Game.SendChat("Good luck and have fun.");
	CreateTimer(3.0, "AutoStartGameDelay()", -1, "AutoStartGameDelay");
}

function AutoStartGameDelay() {
	Timer.Kill();
	Trace("StartCustomGame....\n");
	StartCustomGame(MissionParams);
}

