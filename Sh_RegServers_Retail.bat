regsvr32 /s /u objs10\AZRetail\AGC\AGC.dll
regsvr32 /s objs10\AZRetail\AGC\AGC.dll
regsvr32 /s /u objs10\AZRetail\Test\TCObj\TCObj.dll
regsvr32 /s objs10\AZRetail\Test\TCObj\TCObj.dll
regsvr32 /s /u objs10\AZRetail\Pigs\PigsLib\PigsLib.dll
regsvr32 /s objs10\AZRetail\Pigs\PigsLib\PigsLib.dll
objs10\AZRetail\Lobby\Alllobby -UnRegServer
objs10\AZRetail\Lobby\Alllobby -RegServer
rem objs10\AZRetail\FedSrv\AllSrvMP -UnRegServer
rem objs10\AZRetail\FedSrv\AllSrvMP -RegServer
objs10\AZRetail\FedSrv\AllSrv -UnRegServer
objs10\AZRetail\FedSrv\AllSrv -RegServer
objs10\AZRetail\Club\AllClub -UnRegServer
objs10\AZRetail\Club\AllClub -RegServer
objs10\AZRetail\Pigs\PigAccts\PigAccts -UnRegServer
objs10\AZRetail\Pigs\PigAccts\PigAccts -RegServer %1 %2
copy src\Pigs\PigAccts\PigAccts.ini objs10\AZRetail\Pigs\PigAccts\PigAccts.ini /Y
objs10\AZRetail\Pigs\PigSrv\PigSrv -UnRegServer
objs10\AZRetail\Pigs\PigSrv\PigSrv -RegServer %1 %2


