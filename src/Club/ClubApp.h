/*-------------------------------------------------------------------------
  ClubApp.h
  
  Main class for the club
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
#ifndef _CLUBAPP_H_ 
#define _CLUBAPP_H_ 


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class IClubAppSite;



class IClubClientSite : public IFedMessagingSite
{
public:

// IFedMessagingSite
  virtual HRESULT OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm);
  virtual HRESULT OnSysMessage(FedMessaging * pthis) ;
  virtual void    OnMessageNAK(FedMessaging * pthis, DWORD dwTime, CFMRecipient * prcp) ;
  virtual HRESULT OnNewConnection(FedMessaging * pthis, CFMConnection & cnxn) ;
  virtual HRESULT OnDestroyConnection(FedMessaging * pthis, CFMConnection & cnxn) ;
  virtual HRESULT OnSessionLost(FedMessaging * pthis) ;
  virtual int     OnMessageBox(FedMessaging * pthis, const char * strText, const char * strCaption, UINT nType);
};

class CSQLSiteImpl: public ISQLSite
{
  virtual int  OnMessageBox(const char * strText, const char * strCaption, UINT nType);
};

extern CSQLSiteImpl g_SQLSite;  // needs to be global cause SQL stuff is globally initialized
  

class CClubApp: public Win32App
//$ ASYNCCLUB
// , public ISQLSite2
{
public:
  CClubApp(IClubSite * plas);
  ~CClubApp();

  // Win32App
  virtual bool    OnAssert(const char* psz, const char* pszFile, int line, const char* pszModule);
  virtual void    DebugOutput(const char *psz);
  
  HRESULT         Init();
  int             Run();
  IClubSite * GetSite()
  {
    return m_plas;
  }

  IClubClientSite * GetClientSite()
  {
      return &m_siteClient;
  }

  FedMessaging &  GetFMClients()
  {
    return m_fmClients;
  }

  virtual int     OnMessageBox(const char * strText, const char * strCaption, UINT nType);

  CLUB_COUNTERS * GetCounters()
  {
    return m_pCounters;
  }

  Time GetNow() // time at beginning of current cycle
  {
    return m_timeNow;
  }

  void SetRankInfo(RankInfo * vRankInfo, short cRankInfo)
  {
      assert(m_vRankInfo == NULL);
      m_vRankInfo = vRankInfo;
      m_cRankInfo = cRankInfo;
  }

  RankInfo * GetRankInfo()
  {
    return m_vRankInfo;
  }

  short GetRankInfoCount()
  {
    return m_cRankInfo;
  }

  // BT - 7/15 - CSS Service integration
  char * GetCssServerDomain()
  {
	  return m_szCssServerDomain;
  }

  // BT - 7/15 - CSS Service integration
  char * GetCssClientServicePath()
  {
	  return m_szCssClientServicePath;
  }

  // BT - 7/15 - CSS Service integration
  char * GetCssLobbyServicePath()
  {
	  return m_szCssLobbyServicePath;
  }

  // BT - 7/15 - CSS Service integration
  char * GetCssGameDataServicePath()
  {
	  return m_szCssGameDataServicePath;
  }

  // BT - 7/15 - CSS Integration
  bool IsCssAuthenticationEnabled()
  {
	  return (m_dwCssAuthenticationEnabled > 0) ? true : false;
  }

    /* removed Imago 9/14

  char * GetAuthServer()
  {
    return m_szAuthServer;
  }


  TRef<IZoneAuthServer> GetZoneAuthServer()
  {
    return m_pzas;
  }


  char * GetToken()
  {
    return m_szToken;
  }

    */

//$ ASYNCCLUB
/*
// ISQLSite2
  virtual void OnSQLErrorRecord(SSERRORINFO * perror, OLECHAR * postrError);
  virtual void OnOLEDBErrorRecord(BSTR bstrDescription, GUID guid, DWORD dwHelpContext, 
                                  BSTR bstrHelpFile,    BSTR bstrSource );
*/

private:
  const char *      SzFmMsgHeader() {return "Clients: ";}
  bool              ReadFromRegistry(HKEY & hk, bool bIsString, const char * szItem, void * pValue, DWORD dwDefault);
  void              SetNow()
  {
    m_timeNow = Time::Now();
  }

  IClubSite *       m_plas;
  FedMessaging      m_fmClients;
  IClubClientSite   m_siteClient;

  // *** Perfmon counter stuff ***
  CPerfShare        m_perfshare;
  CLUB_COUNTERS *   m_pCounters;
  Time              m_timeNow;

  //TRef<IZoneAuthServer> m_pzas; //Imago 9/14
  RankInfo*         m_vRankInfo;
  short             m_cRankInfo;
  //char              m_szAuthServer[64];
  //char              m_szToken[24]; // sizeof(_ZONETICKET_TOKEN.szToken)

  // BT - 7/15 CSS Integration.
  char				m_szCssServerDomain[2064];
  char				m_szCssClientServicePath[2064];
  char				m_szCssLobbyServicePath[2064];
  char				m_szCssGameDataServicePath[2064];
  DWORD				m_dwCssAuthenticationEnabled;

/*
  //$ ASYNCCLUB
  CSQLCore          m_sql;
  DWORD             m_dwThreadsNotify;
  DWORD             m_dwThreadsSilent;
  CComBSTR          m_bstrSQLConfig;
*/
};

extern CClubApp * g_pClubApp;  

#endif
