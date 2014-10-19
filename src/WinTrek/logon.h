#ifndef _logon_h_
#define _logon_h_

//////////////////////////////////////////////////////////////////////////////
//
// logon
//
//////////////////////////////////////////////////////////////////////////////

class LogonSite : public IObject {
public:
    virtual void OnLogon(const ZString& strName, const ZString& strPassword, BOOL fRememberPW) = 0;
    virtual void OnAbort() = 0;
};

enum LogonType
{
  LogonAllegianceZone,
  LogonFreeZone,
  LogonLAN,
};

TRef<IPopup> CreateLogonPopup(Modeler* pmodeler, LogonSite* psite, LogonType lt, 
                              LPCWSTR szPrompt, LPCWSTR szName, LPCWSTR szPW, BOOL fRememberPW);

#endif
