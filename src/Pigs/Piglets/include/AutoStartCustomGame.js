/////////////////////////////////////////////////////////////////////////////
// AutoStartCustomGame.js : Code needed to automatically start a game once the
// minimum number of players per team have joined the game. - Changes mission params first!
//

var MissionParams;

/////////////////////////////////////////////////////////////////////////////
// Description: Creates a timer to check for the presence of the minimum
// number of players in each team.
//
// Parameters:
//   cMinTeamPlayers - The minimum number of players per team required to
// start the game. This is passed-in as a convenience, since I'm not sure
// that this value can be derived from the current object model.
//
// See Also: AutoStartGame_Tick
//
function AutoStartGame(objParams)
{
  if (PigState_WaitingForMission != PigState)
    throw "Cannot call AutoStartGame unless PigState is PigState_WaitingForMission";

  MissionParams = objParams;
  CreateTimer(1.0, "AutoStartGame_Tick()", -1, "AutoStartGameTimer");
}


/////////////////////////////////////////////////////////////////////////////
// Description: Called for each interval of the AutoStartGameTimer.
//
// Parameters:
//   cMinTeamPlayers - The minimum number of players per team required to
// start the game. This is passed-in as a convenience, since I'm not sure
// that this value can be derived from the current object model.
//
// Checks for the minimum number of players on each team of the game. When
// this is true, the timer is killed and the game is started.
//
// See Also: AutoStartGame
//
function AutoStartGame_Tick()
{
	var objParams = MissionParams;
  // Check each team for the minimum number of players
  for (var it = new Enumerator(Game.Teams); !it.atEnd(); it.moveNext())
    if (it.item().Ships.Count < objParams.MinTeamPlayers)
      return;

  // Kill the timer
  Timer.Kill();

  // Start the game
  Trace("Trying to start custom game....\n");
  StartCustomGame(objParams);
}

