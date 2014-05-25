/////////////////////////////////////////////////////////////////////////////
// Entry point simply calls the main function.

main(WScript.Arguments, WScript.Arguments.Count());


/////////////////////////////////////////////////////////////////////////////
// Global Declarations

var objSession;


/////////////////////////////////////////////////////////////////////////////
// Description: Parses the command line parameters and validates the usage.
//
function main(args, cArgs)
{
  // Validate the command line arguments
  if (3 != cArgs && 2 != cArgs)
    return Syntax();

  // Get the specified behavior type
  var strServer   = args(0);
  var nCount      = args(1);
  var strBehavior = (3 == cArgs) ? args(2) : "Default";

  // Create the Session Information object
  //var objInfo = new ActiveXObject("Pigs.SessionInfo");
  //objInfo.ApplicationName = WScript.ScriptFullName;

  // Create a utility object here
  var objUtil = new ActiveXObject("TCUtil");

  // Connect to the Pig Server
  objSession = objUtil.CreateObject("Pigs.Session", strServer);
  //objSession.SessionInfo = objInfo;

  WScript.Echo("Creating " + nCount + " pigs with the '" + strBehavior + "' behavior on " + strServer + "...");
  for (var i = 0; i < nCount; i++)
  {
    // Create a single pig
    var objPig = objSession.CreatePig(strBehavior);
    WScript.Echo("1 pig named \"" + objPig + "\" created.");
  }

  WScript.Echo(nCount + " pigs created.");

}


/////////////////////////////////////////////////////////////////////////////
// Displays the usage syntax of this script
//
function Syntax()
{
  var str  = "\nUsage:\n\n";
      str +=   "  CreatePig <computer_name> <count> [behavior]";
  WScript.Echo(str);
}


