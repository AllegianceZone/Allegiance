regsvr32 /s /u objs14\AZRetail\AGC\AGC.dll
regsvr32 /s objs14\AZRetail\AGC\AGC.dll
regsvr32 /s /u objs14\AZRetail\Test\TCObj\TCObj.dll
regsvr32 /s objs14\AZRetail\Test\TCObj\TCObj.dll
regsvr32 /s /u objs14\AZRetail\Pigs\PigsLib\PigsLib.dll
regsvr32 /s objs14\AZRetail\Pigs\PigsLib\PigsLib.dll
objs14\AZRetail\Lobby\Alllobby -UnRegServer
objs14\AZRetail\Lobby\Alllobby -RegServer
rem objs14\AZRetail\FedSrv\AllSrvMP -UnRegServer
rem objs14\AZRetail\FedSrv\AllSrvMP -RegServer
objs14\AZRetail\FedSrv\AllSrv -UnRegServer
objs14\AZRetail\FedSrv\AllSrv -RegServer
objs14\AZRetail\Club\AllClub -UnRegServer
objs14\AZRetail\Club\AllClub -RegServer
objs14\AZRetail\Pigs\PigAccts\PigAccts -UnRegServer
objs14\AZRetail\Pigs\PigAccts\PigAccts -RegServer %1 %2
copy src\Pigs\PigAccts\PigAccts.ini objs14\AZRetail\Pigs\PigAccts\PigAccts.ini /Y
objs14\AZRetail\Pigs\PigSrv\PigSrv -UnRegServer
objs14\AZRetail\Pigs\PigSrv\PigSrv -RegServer %1 %2


