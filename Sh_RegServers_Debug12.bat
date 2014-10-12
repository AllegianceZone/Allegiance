regsvr32 /s /u objs12\AZDebug\AGC\AGC.dll
regsvr32 /s objs12\AZDebug\AGC\AGC.dll
regsvr32 /s /u objs12\AZDebug\Test\TCObj\TCObj.dll
regsvr32 /s objs12\AZDebug\Test\TCObj\TCObj.dll
regsvr32 /s /u objs12\AZDebug\Pigs\PigsLib\PigsLib.dll
regsvr32 /s objs12\AZDebug\Pigs\PigsLib\PigsLib.dll
objs12\AZDebug\Lobby\Alllobby -UnRegServer
objs12\AZDebug\Lobby\Alllobby -RegServer
rem objs12\AZDebug\FedSrv\AllSrvMP -UnRegServer
rem objs12\AZDebug\FedSrv\AllSrvMP -RegServer
objs12\AZDebug\FedSrv\AllSrv -UnRegServer
objs12\AZDebug\FedSrv\AllSrv -RegServer
objs12\AZDebug\Club\AllClub -UnRegServer
objs12\AZDebug\Club\AllClub -RegServer
objs12\AZDebug\Pigs\PigAccts\PigAccts -UnRegServer
objs12\AZDebug\Pigs\PigAccts\PigAccts -RegServer %1 %2
copy src\Pigs\PigAccts\PigAccts.ini objs12\AZDebug\Pigs\PigAccts\PigAccts.ini /Y
objs12\AZDebug\Pigs\PigSrv\PigSrv -UnRegServer
objs12\AZDebug\Pigs\PigSrv\PigSrv -RegServer %1 %2


