/*-------------------------------------------------------------------------
  pch.h
  
  Precompiled header for AllClub
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _ALLCLUB_PCH_
#define _ALLCLUB_PCH_

#define ALLCLUB_MODULE 1

//Astn Imago 9/14
#ifndef WINVER
#define WINVER 0x0501 
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <windows.h>
#include <stdlib.h>
#include <memory.h>
#include <sqlext.h>
//#include <dplobby.h> Imago 9/14

// This also includes <atlbase.h> and <atlcom.h>
#include "AllClubModule.h"

#define __MODULE__ "AllClub"
#include <zlib.h>
#include <utility.h>

#include <TCLib.h>
#include <TCAtl.h>
#include <..\TCAtl\ObjectMap.h>
#include <..\TCAtl\UserAcct.h>
#include <..\TCLib\TCThread.h>

#include "..\fedsrv\fedsrvsql.h"

#include <guids.h>
#include "resource.h"
#include "zone.h" 
#include "messagecore.h"
#include "ClubMessages.h"
#include "sharemem.h"
#include "ClubCounters.h"
//#include "zauth.h" //Imago 9/16

// BT - 7/15 - Enable CssSoap to be shared with the club server too.
#include "..\lobby\messagesls.h"

//$ ASYNCCLUB
//#include "clubqueries2.h"

#include "regkey.h" // KGJV added

#include "ClubApp.h"
#include "ClubClient.h"
//#include "StaticData.h" //- obsolete


/////////////////////////////////////////////////////////////////////////////
// This is included very last since it conflicts with <atlwin.h>, included
// indirectly by <TCAtl.h>.
//
#include <windowsx.h> // For GlobalAllocPtr and GlobalFreePtr


/////////////////////////////////////////////////////////////////////////////

#endif
