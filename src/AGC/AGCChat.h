#ifndef __AGCChat_h__
#define __AGCChat_h__

/////////////////////////////////////////////////////////////////////////////
// AGCChat.h : Declaration of things common to chatting from AGC.
//

#include "..\igc\igc.h"
#include "resource.h"
#include <agc.h>


/////////////////////////////////////////////////////////////////////////////
// Command Name Resolution

inline HRESULT FindCommandName(const wchar_t* pszCommand, CommandID* pID,
  REFCLSID clsid, REFIID iid)
{
  // Search for the specified command string
  assert(pszCommand);
  for (CommandID i = 0; i < c_cidMax; ++i)
  {
    if (!_wcsicmp(pszCommand, c_cdAllCommands[i].szVerb))
    {
      *pID = i;
      return S_OK;
    }
  }

  // Specified command string was not found
  return AtlReportError(clsid, IDS_E_COMMANDNAME, iid,
    MAKE_HRESULT(1, FACILITY_ITF, IDS_E_COMMANDNAME),
    GetAGCGlobal()->GetResourceInstance());
}

/////////////////////////////////////////////////////////////////////////////

#endif // !__AGCChat_h__
