<pigbehavior language="JScript">

<script src="include\AutoStartCustomGame.js"/>

<script>
<![CDATA[

/////////////////////////////////////////////////////////////////////////////
// Beginning of script
//

var GameName = "Bot DeathMatch";

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
      UpdatesPerSecond = 20;
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
  objParams.TeamKills = 20;
  CreateMission("Imago-PC","192.168.2.2",objParams);
  //CreateMission("azbuildslave","191.239.1.217",objParams);

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
     	Properties("JoinTimer").Kill();
     	Trace("killed timer, Attempting to JoinTeam\n");
      	JoinTeam();	
}

/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Joins a random team.
function OnStateTeamList(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
  // Avoid repeated attempts to join a team
  if (PigState_JoiningTeam == eStatePrevious)
  {
    Trace("PigState_JoiningTeam so QuitGame()\n");
    QuitGame();
  }
  else
  {
    try
    {
     Trace("Attempting to JoinTeam\n");
      JoinTeam();
    }
    catch (e)
    {
      Trace("Attempting to QuitGame\n");
      QuitGame();
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// Selects the best hull from a collection of hulls.  Scans the list, noting
// the index of the Fighter and the Interceptor.  If a Fighter was found, it
// returns that index.  Otherwise, the index of the Interceptor is returned.
// If an Interceptor is not found, returns 0, which should be a scout or a
// recovery ship.
//
function SelectBestHull(objHullTypes)
{
  var FighterHull = -1, InterceptorHull = 0;
  var e = new Enumerator(objHullTypes)

  // loop thru collection
  var strTrace = "Hull types:\n";
  for (var i = 0; !e.atEnd(); e.moveNext(), ++i)
  {
    hull = e.item();
    strTrace += "  " +  i + ". " + hull.Name + "\n";

    // search for "Interceptor" in the name
    if (hull.Name.search("Interceptor") != -1)
      InterceptorHull = i;

    // search for "Fighter" in the name
    if (hull.Name.search("Fighter") != -1)
      FighterHull = i;    
  }
  strTrace += "Fighter: " + FighterHull + " Interceptor: " + InterceptorHull + "\n";

  // look for valid Fighter index
  if (FighterHull != -1)
  {
    // fighter found, return index
    Trace(strTrace + "Selecting fighter.\n");
    return FighterHull;
  }
  else
  {
    // interceptor found, or default, return index
    Trace (strTrace + "Selecting interceptor.\n");
    return InterceptorHull;
  }
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Launches the pig as soon as it becomes docked.
function OnStateDocked(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
  // Kill the flying timer, if one exists
  if ("object" == typeof(Properties("FlyingTimer")))
    Properties("FlyingTimer").Kill();

  try
  {
    // Get the list of hull types
    var objHullTypes = HullTypes;
    var cHullTypes = objHullTypes.Count;
    Host.Trace("\nOnStateDocked: cHullTypes = " + cHullTypes + "\n");

    // Select and buy the best hull available
    var iHull = SelectBestHull(objHullTypes);
    if (cHullTypes)
      Ship.BuyHull(objHullTypes(iHull));
  }
  catch (e)
  {
    Trace("\n" + e.description + "\n");
  }

  // Launch into space
  Trace("Launching....\n");
  Launch();
}


/////////////////////////////////////////////////////////////////////////////
// Handles state transition. Currently just outputs debug text.
function OnStateFlying(eStatePrevious)
{
	DisplayStateTransition(eStatePrevious);
  try
  {
    // Set the ship's throttle to 100%
    Throttle = 100;

    // Create a timer to reset the throttle
    var fDuration = 5.0 + (Random() % 5);
    CreateTimer(fDuration, "OnInitialThrottle()", -1, "FlyingTimer");
  }
  catch (e)
  {
    Trace("Error in OnStateFlying():\n\t" + e.description);
  }
}


/////////////////////////////////////////////////////////////////////////////
function OnInitialThrottle()
{
  // Set the ship's throttle to 0%
  Ship.Throttle = 0;

  // Set the timer's interval to 2 seconds
  Timer.Interval = 2.0;
  Timer.ExpirationExpression = "CorrectOrbit()";
}


/////////////////////////////////////////////////////////////////////////////
function CorrectOrbit()
{
  try
  {
    
    //new stuff 9/10
    if (HullType.Name.search("Lifepod") != -1)
	CommitSuicide();
    if (Position.Subtract(Ship.Sector.Stations(0).Position).Length > 5000)
	CommitSuicide();
    //end new stuff

    Ship.Face(Ship.Sector.Stations(0), "ThrustOrbit();");
    return;
  }
  catch (e)
  {
    Trace("Error in CorrectOrbit():\n\t" + e.description);
  }
}


/////////////////////////////////////////////////////////////////////////////
function ThrustOrbit()
{
	try
	{
		// Stop the ship
		AllStop();

		// Thrust to the left
		Thrust(ThrustLeft, ThrustForward);

		// Toggle the firing of the weapon
		FireWeapon(true);
		
		// Check for empty Ammo
		if (!Ammo || Energy < 200)
			CommitSuicide();
	}
	catch (e)
	{
    Trace("Error in ThrustOrbit():\n\t" + e.description);
	}
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

