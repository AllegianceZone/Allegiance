#pragma once


//imago 8/14 - changed back - again, this was an undocument change which breaks our license agreement
#ifdef  _ALLEGIANCE_DEV_
#define ALLEGIANCE_REGISTRY_KEY_ROOT   L"Software\\Microsoft\\Microsoft Games\\Allegiance\\Dev"
#define ALLEGIANCE_REGISTRY_KEY_ROOTA   L"Software\\Microsoft\\Microsoft Games\\Allegiance\\Dev"
#else
#ifdef _ALLEGIANCE_PROD_
#define ALLEGIANCE_REGISTRY_KEY_ROOT   L"Software\\Microsoft\\Microsoft Games\\Allegiance\\1.2"
#define ALLEGIANCE_REGISTRY_KEY_ROOTA   L"Software\\Microsoft\\Microsoft Games\\Allegiance\\1.2"
#else
#define ALLEGIANCE_REGISTRY_KEY_ROOT   L"Software\\Microsoft\\Microsoft Games\\Allegiance\\1.3"
#define ALLEGIANCE_REGISTRY_KEY_ROOTA   "Software\\Microsoft\\Microsoft Games\\Allegiance\\1.3"
#endif
#endif

// KGJV - added lobby & club - Imago 10/1
#if !defined(ALLSRV_STANDALONE)
  #define HKLM_AllSrvUI      ALLEGIANCE_REGISTRY_KEY_ROOT L"\\Server"
//#define HKLM_AllLobby		 "SYSTEM\\CurrentControlSet\\Services\\AllLobby"
  #define HKLM_AllLobby		 ALLEGIANCE_REGISTRY_KEY_ROOT L"\\AllLobby"
  #define HKLM_AllClub		 ALLEGIANCE_REGISTRY_KEY_ROOT L"\\AllClub"
#define HKLM_AllClubA		 ALLEGIANCE_REGISTRY_KEY_ROOTA "\\AllClub"
//#define HKLM_AllClub       "SYSTEM\\CurrentControlSet\\Services\\AllClub"
#else // !defined(ALLSRV_STANDALONE)
  const char c_szSvcName[] = "AllSrv";
  #define HKLM_FedSrv        ALLEGIANCE_REGISTRY_KEY_ROOT L"\\Server"
  #define HKLM_FedSrvA        ALLEGIANCE_REGISTRY_KEY_ROOTA "\\Server"
  #define HKLM_AllSrvUI      HKLM_FedSrv
  #define HKLM_AllLobby		 ALLEGIANCE_REGISTRY_KEY_ROOT L"\\AllLobby"
  #define HKLM_AllClub		 ALLEGIANCE_REGISTRY_KEY_ROOT L"\\AllClub"
  #define HKLM_AllClubA		 ALLEGIANCE_REGISTRY_KEY_ROOTA "\\AllClub"
#endif // !defined(ALLSRV_STANDALONE)


