
/////////////////////////////////////////////////////////////////////////////
// Create the Session Information object

//var objInfo = new ActiveXObject("Pigs.SessionInfo");
//objInfo.ApplicationName = WScript.ScriptFullName;


/////////////////////////////////////////////////////////////////////////////
// Connect to the Pig Server

var objSession = new ActiveXObject("Pigs.Session");
//objSession.SessionInfo = objInfo;


/////////////////////////////////////////////////////////////////////////////
// Get the pig

var objPig = objSession.Pigs("botpilot1");
WScript.Echo(objPig + ": State is \"" + objPig.PigStateName + "\"");

// Thrust the pig forward
 objPig.Ship.Attack("botpilot2");
