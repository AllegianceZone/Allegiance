regsvr32 /s /u objs12\AZRetail\AGC\AGC.dll
regsvr32 /s objs12\AZRetail\AGC\AGC.dll
regsvr32 /s /u objs12\AZRetail\Test\TCObj\TCObj.dll
regsvr32 /s objs12\AZRetail\Test\TCObj\TCObj.dll
regsvr32 /s /u objs12\AZRetail\Pigs\PigsLib\PigsLib.dll
regsvr32 /s objs12\AZRetail\Pigs\PigsLib\PigsLib.dll
objs12\AZRetail\Lobby\Alllobby -UnRegServer
objs12\AZRetail\Lobby\Alllobby -RegServer
rem objs12\AZRetail\FedSrv\AllSrvMP -UnRegServer
rem objs12\AZRetail\FedSrv\AllSrvMP -RegServer
objs12\AZRetail\FedSrv\AllSrv -UnRegServer
objs12\AZRetail\FedSrv\AllSrv -RegServer
objs12\AZRetail\Club\AllClub -UnRegServer
objs12\AZRetail\Club\AllClub -RegServer
objs12\AZRetail\Pigs\PigAccts\PigAccts -UnRegServer
objs12\AZRetail\Pigs\PigAccts\PigAccts -RegServer %1 %2
copy src\Pigs\PigAccts\PigAccts.ini objs12\AZRetail\Pigs\PigAccts\PigAccts.ini /Y
objs12\AZRetail\Pigs\PigSrv\PigSrv -UnRegServer
objs12\AZRetail\Pigs\PigSrv\PigSrv -RegServer %1 %2


