regsvr32 /s /u objs14\AZDebug\AGC\AGC.dll
regsvr32 /s objs14\AZDebug\AGC\AGC.dll
regsvr32 /s /u objs14\AZDebug\Test\TCObj\TCObj.dll
regsvr32 /s objs14\AZDebug\Test\TCObj\TCObj.dll
regsvr32 /s /u objs14\AZDebug\Pigs\PigsLib\PigsLib.dll
regsvr32 /s objs14\AZDebug\Pigs\PigsLib\PigsLib.dll
objs14\AZDebug\Lobby\Alllobby -UnRegServer
objs14\AZDebug\Lobby\Alllobby -RegServer
rem objs14\AZDebug\FedSrv\AllSrvMP -UnRegServer
rem objs14\AZDebug\FedSrv\AllSrvMP -RegServer
objs14\AZDebug\FedSrv\AllSrv -UnRegServer
objs14\AZDebug\FedSrv\AllSrv -RegServer
objs14\AZDebug\Club\AllClub -UnRegServer
objs14\AZDebug\Club\AllClub -RegServer
objs14\AZDebug\Pigs\PigAccts\PigAccts -UnRegServer
objs14\AZDebug\Pigs\PigAccts\PigAccts -RegServer %1 %2
copy src\Pigs\PigAccts\PigAccts.ini objs14\AZDebug\Pigs\PigAccts\PigAccts.ini /Y
objs14\AZDebug\Pigs\PigSrv\PigSrv -UnRegServer
objs14\AZDebug\Pigs\PigSrv\PigSrv -RegServer %1 %2


