#pragma once

#ifndef __AGCWinApp_h__
#define __AGCWinApp_h__

/////////////////////////////////////////////////////////////////////////////
// AGCWinApp.h : Declaration of the CAGCWinApp class.
//

#include <AGC.h>
#include <..\TCLib\WinApp.h>


/////////////////////////////////////////////////////////////////////////////
//
class CAGCWinApp : public TCWinAppDLL
{
// Attributes
public:
  void SetDebugHook(IAGCDebugHook* pdh)
  {
    m_spDebugHook = pdh;
  }

// Overrides
public:
  #ifdef _DEBUG
	virtual void DebugOutput(const wchar_t* psz)
    {
		if (NULL != m_spDebugHook) {
			m_spDebugHook->DebugOutput(psz);
		} else {
			char mbstring[4096];
			wcstombs(mbstring, psz, wcslen(psz));
			TCWinAppDLL::DebugOutput(mbstring);
		}
    }

	virtual bool OnAssert(const wchar_t* psz, const wchar_t* pszFile, int line, const wchar_t* pszModule)
    {
		if (NULL != m_spDebugHook) {
			return !!m_spDebugHook->OnAssert(psz, pszFile, line, pszModule);
		} else {
			char mbstring[4096];
			wcstombs(mbstring, psz, wcslen(psz));
			char mbpath[MAX_PATH+128];
			wcstombs(mbpath, pszFile, wcslen(pszFile));
			char mbmodule[MAX_PATH + 128];
			wcstombs(mbmodule, pszModule, wcslen(pszModule));
			return TCWinAppDLL::OnAssert(mbstring, mbpath, line, mbmodule);
		}
    }

    virtual void OnAssertBreak()
    {
      if (NULL != m_spDebugHook)
        m_spDebugHook->OnAssertBreak();
      else
        TCWinAppDLL::OnAssertBreak();
    }
  #endif // _DEBUG

// Data Members
protected:
  IAGCDebugHookPtr m_spDebugHook;
};


/////////////////////////////////////////////////////////////////////////////
// External Reference

extern CAGCWinApp g_app;


/////////////////////////////////////////////////////////////////////////////

#endif // !__AGCWinApp_h__
