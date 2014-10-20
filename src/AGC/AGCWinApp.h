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
			size_t   i;
			char      *pMBBuffer = (char *)malloc(2048);
			wcstombs_s(&i, pMBBuffer, (size_t)2048, psz, (size_t)2048);
			TCWinAppDLL::DebugOutput(pMBBuffer);
			if (pMBBuffer)
				free(pMBBuffer);
		}
    }

	virtual bool OnAssert(const wchar_t* psz, const wchar_t* pszFile, int line, const wchar_t* pszModule)
    {
		if (NULL != m_spDebugHook) {
			return !!m_spDebugHook->OnAssert(psz, pszFile, line, pszModule);
		} else {
			size_t   i1;
			char      *pMBBuffer1 = (char *)malloc(2048);
			wcstombs_s(&i1, pMBBuffer1, (size_t)2048, psz, (size_t)2048);
			size_t   i2;
			char      *pMBBuffer2 = (char *)malloc(400);
			wcstombs_s(&i2, pMBBuffer2, (size_t)400, pszFile, (size_t)400);
			size_t   i3;
			char      *pMBBuffer3 = (char *)malloc(400);
			wcstombs_s(&i3, pMBBuffer3, (size_t)400, pszModule, (size_t)400);
			return TCWinAppDLL::OnAssert(pMBBuffer1, pMBBuffer2, line, pMBBuffer3);
			if (pMBBuffer1)
				free(pMBBuffer1);
			if (pMBBuffer2)
				free(pMBBuffer2);
			if (pMBBuffer3)
				free(pMBBuffer3);
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
