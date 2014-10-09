regsvr32 /s /u AGC.dll
regsvr32 /s AGC.dll
regsvr32 /s /u TCObj.dll
regsvr32 /s TCObj.dll
regsvr32 /s /u PigsLib.dll
regsvr32 /s PigsLib.dll
Alllobby -UnRegServer
Alllobby -RegServer
rem AllSrvMP -UnRegServer
rem AllSrvMP -RegServer
AllSrv -UnRegServer
AllSrv -RegServer
AllClub -UnRegServer
AllClub -RegServer
PigAccts -UnRegServer
PigAccts -RegServer %1 %2
PigSrv -UnRegServer
PigSrv -RegServer %1 %2

