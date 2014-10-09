regsvr32 /s /u objs10\AZDebug\AGC\AGC.dll
regsvr32 /s objs10\AZDebug\AGC\AGC.dll
regsvr32 /s /u objs10\AZDebug\Test\TCObj\TCObj.dll
regsvr32 /s objs10\AZDebug\Test\TCObj\TCObj.dll
regsvr32 /s /u objs10\AZDebug\Pigs\PigsLib\PigsLib.dll
regsvr32 /s objs10\AZDebug\Pigs\PigsLib\PigsLib.dll
objs10\AZDebug\Lobby\Alllobby -UnRegServer
objs10\AZDebug\Lobby\Alllobby -RegServer
rem objs10\AZDebug\FedSrv\AllSrvMP -UnRegServer
rem objs10\AZDebug\FedSrv\AllSrvMP -RegServer
objs10\AZDebug\FedSrv\AllSrv -UnRegServer
objs10\AZDebug\FedSrv\AllSrv -RegServer
objs10\AZDebug\Club\AllClub -UnRegServer
objs10\AZDebug\Club\AllClub -RegServer
objs10\AZDebug\Pigs\PigAccts\PigAccts -UnRegServer
objs10\AZDebug\Pigs\PigAccts\PigAccts -RegServer %1 %2
copy src\Pigs\PigAccts\PigAccts.ini objs10\AZDebug\Pigs\PigAccts\PigAccts.ini /Y
objs10\AZDebug\Pigs\PigSrv\PigSrv -UnRegServer
objs10\AZDebug\Pigs\PigSrv\PigSrv -RegServer %1 %2


