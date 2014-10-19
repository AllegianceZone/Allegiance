#include "pch.h"
#include "regkey.h"

//Imago 7/10
#include <dbghelp.h>

//////////////////////////////////////////////////////////////////////////////
//
// Some assertion functions
//
//////////////////////////////////////////////////////////////////////////////

Win32App *g_papp;

	// Patch for SetUnhandledExceptionFilter 
const BYTE PatchBytes[5] = { 0x33, 0xC0, 0xC2, 0x04, 0x00 };

	// Original bytes at the beginning of SetUnhandledExceptionFilter 
BYTE OriginalBytes[5] = {0};

//Imago 6/10
int Win32App::GenerateDump(EXCEPTION_POINTERS* pExceptionPointers)
{
    BOOL bMiniDumpSuccessful;
    wchar_t szPathName[MAX_PATH] = L""; 
	GetModuleFileName(NULL, szPathName, MAX_PATH);
	wchar_t* p1 = wcsrchr(szPathName, '\\');
	wchar_t* p = wcsrchr(szPathName, '\\');
	if (!p)
		p = szPathName;
	else
		p++;
	if (!p1)
		p1 = L"mini";
	else
		p1++;
	ZString zApp = p1;
    DWORD dwBufferSize = MAX_PATH;
    HANDLE hDumpFile;
    SYSTEMTIME stLocalTime;
    MINIDUMP_EXCEPTION_INFORMATION ExpParam;

    GetLocalTime( &stLocalTime );
    
   wcscpy(p, (PCC)zApp);
   
   ZVersionInfo vi; ZString zInfo = vi.GetFileVersionString();
   swprintf(p + zApp.GetLength(), L"-%s-%04d%02d%02d%02d%02d%02d-%ld-%ld.dmp", (PCC)zInfo,
               stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
               stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, 
               GetCurrentProcessId(), GetCurrentThreadId());
   
    hDumpFile = CreateFile(szPathName, GENERIC_READ|GENERIC_WRITE, 
                FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

    ExpParam.ThreadId = GetCurrentThreadId();
    ExpParam.ExceptionPointers = pExceptionPointers;
    ExpParam.ClientPointers = TRUE;

	MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)
		(MiniDumpWithDataSegs		| 
		MiniDumpWithHandleData		|
		MiniDumpWithThreadInfo		| 
		MiniDumpWithUnloadedModules |
		MiniDumpWithProcessThreadData); 

	//
	//MiniDumpWithPrivateReadWriteMemory | 
	//MiniDumpWithFullMemoryInfo | 
	//

    bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
                    hDumpFile, mdt, &ExpParam, NULL, NULL);


    return EXCEPTION_EXECUTE_HANDLER;
}

//lazy...or stupid... is this even hit?
int GenerateDump(EXCEPTION_POINTERS* pExceptionPointers)
{
    BOOL bMiniDumpSuccessful;
    wchar_t szPathName[MAX_PATH] = L""; 
	GetModuleFileName(NULL, szPathName, MAX_PATH);
	wchar_t* p1 = wcsrchr(szPathName, '\\');
	wchar_t* p = wcsrchr(szPathName, '\\');
	if (!p)
		p = szPathName;
	else
		p++;
	if (!p1)
		p1 = L"mini";
	else
		p1++;
	ZString zApp = p1;
    DWORD dwBufferSize = MAX_PATH;
    HANDLE hDumpFile;
    SYSTEMTIME stLocalTime;
    MINIDUMP_EXCEPTION_INFORMATION ExpParam;

    GetLocalTime( &stLocalTime );
    
   wcscpy(p, (PCC)zApp);
   ZVersionInfo vi; ZString zInfo = vi.GetFileVersionString();
   swprintf( p+zApp.GetLength(),L"-%s-%04d%02d%02d%02d%02d%02d-%ld-%ld.dmp",(PCC)zInfo,
               stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
               stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, 
               GetCurrentProcessId(), GetCurrentThreadId());
   
    hDumpFile = CreateFile(szPathName, GENERIC_READ|GENERIC_WRITE, 
                FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

    ExpParam.ThreadId = GetCurrentThreadId();
    ExpParam.ExceptionPointers = pExceptionPointers;
    ExpParam.ClientPointers = TRUE;

	MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)
		(MiniDumpWithDataSegs		| 
		MiniDumpWithHandleData		|
		MiniDumpWithThreadInfo		| 
		MiniDumpWithUnloadedModules |
		MiniDumpWithProcessThreadData); 

    bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), 
                    hDumpFile, mdt, &ExpParam, NULL, NULL);

    return EXCEPTION_EXECUTE_HANDLER;
}


//////////////////////////////////////////////////////////////////////////////
//
// Some assertion functions
//
//////////////////////////////////////////////////////////////////////////////
void ZAssertImpl(bool bSucceeded, const wchar_t* psz, const wchar_t* pszFile, int line, const wchar_t* pszModule)
{
    if (!bSucceeded) {
        //
        // Just in case this was a Win32 error get the last error
        //

        DWORD dwError = GetLastError();

        if (!g_papp) {
			// Imago removed asm (x64) on ?/?, integrated with mini dump on 6/10
			__try {
				(*(int*)0) = 0;
			}
			__except(GenerateDump(GetExceptionInformation())) {}
        } else if (g_papp->OnAssert(psz, pszFile, line, pszModule)) {
            g_papp->OnAssertBreak();
        }
    }
}

// mmf added code for chat logging
// mmf 7/15 changed creation flag on chat file so other processes can read from it
// avalanche + mmf 03/22/07 (bugs 108 and 109) place chat logs in logs folder, use \r\n

HANDLE chat_logfile = NULL;
wchar_t logFileName[MAX_PATH + 21];

void InitializeLogchat()
{
	HKEY hKey;
	DWORD dwType;
	wchar_t szValue[20];
	DWORD cbValue = sizeof(szValue);
	bool bLogChat = false;

	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey))
	{
		//Imago fixed this but is still confused why it's not a dword.
		if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, L"LogChat", NULL, &dwType, (unsigned char*)&szValue, &cbValue))		
			bLogChat = (wcscmp(szValue, L"1") == 0);
		::RegCloseKey(hKey);
	}


	if (bLogChat)
	{
		time_t longTime;
		time(&longTime);
		tm* t = new tm;
	//	tm* t = localtime(&longTime);
		localtime_s(t, &longTime);

		// char logFileName[MAX_PATH + 21]; make this global so chat can open and close it
		// turns out this is not needed but leaving it here instead of moving it again
		GetModuleFileName(NULL, logFileName, MAX_PATH);
		wchar_t* p = wcsrchr(logFileName, '\\');
		if (!p)
			p = logFileName;
		else
			p++;

		wcscpy(p, L"logs\\");

		if (!CreateDirectory(logFileName, NULL))
		{
			if (GetLastError() == ERROR_PATH_NOT_FOUND)
			{
				debugf(L"Unable to create chat log directory %s\n",logFileName);
			}
		}

		// mmf 1/17/08 fixed month
		swprintf(p + 5, L"chat_%02d-%02d-%02d-%02d%02d%02d.txt", (t->tm_year - 100), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

		// mmf changed 3 param from 0 to FILE_SHARE_READ
		chat_logfile =
			CreateFile(
				logFileName,
				GENERIC_WRITE,
				FILE_SHARE_READ,
				NULL,
				OPEN_ALWAYS,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
				NULL
			);
		delete t;

		//Imago moved inside bLogChat
		if (chat_logfile == NULL) debugf(L"Unable to create chat_logfile %s\n",logFileName);
	}
}

void TerminateLogchat()
{
	if (chat_logfile) {
		CloseHandle(chat_logfile);
        chat_logfile = NULL;
    }
}

void logchat(const wchar_t* strText)
{
	// unravel this from debugf-ZDebugOutputImpl-DebugOutput
    const size_t size = 512;
	wchar_t         bfr[size];
	int length;

	time_t  longTime;
    time(&longTime);
	tm* t = new tm;
//    tm*             t = localtime(&longTime);
	localtime_s(t, &longTime);

	length = wcslen(strText);

	// don't log if text is bigger than buffer, we don't want to log these long 'spam' chat's anyway
	if (chat_logfile && (length < 490)) {
		swprintf(bfr, L"%02d/%02d/%02d %02d:%02d:%02d: %s\r\n",
            (t->tm_mon + 1), t->tm_mday, (t->tm_year - 100), t->tm_hour, t->tm_min, t->tm_sec, strText);
        DWORD nBytes;
        ::WriteFile(chat_logfile, bfr, wcslen(bfr), &nBytes, NULL);
	}
	delete t;
}

// end mmf chat logging code


void ZDebugOutputImpl(const wchar_t *psz)
{
    if (g_papp)
        g_papp->DebugOutput(psz);
    else
        ::OutputDebugString(psz);
}
HANDLE g_logfile = NULL;

extern int g_outputdebugstring = 0;  // mmf temp change, control outputdebugstring call with reg key

void retailf(const wchar_t* format, ...)
{
    if (g_bOutput)
    {
        const size_t size = 2048; //Avalance: Changed to log longer messages. (From 512)
		wchar_t         bfr[size];

        va_list vl;
        va_start(vl, format);
        _vsnwprintf_s(bfr, size, (size-1), format, vl); //Avalanche: Fix off by one error. 
        va_end(vl);

        ZDebugOutputImpl(bfr);
    }
}

extern bool g_bOutput = true;

// mmf log to file on SRVLOG define as well as _DEBUG
#ifdef _DEBUG
#define SRVLOG
#endif

#ifdef SRVLOG // mmf changed this from _DEBUG

void ZWarningImpl(bool bSucceeded, const wchar_t* psz, const wchar_t* pszFile, int line, const wchar_t* pszModule)
    {
        if (!bSucceeded) {
            debugf(L"%s(%d) : ShouldBe failed: '%s'\n", pszFile, line, psz);
        }
    }

	bool ZFailedImpl(HRESULT hr, const wchar_t* pszFile, int line, const wchar_t* pszModule)
    {
        bool bFailed = FAILED(hr);
        ZAssertImpl(!bFailed, L"Function Failed", pszFile, line, pszModule);
        return bFailed;
    }

	bool ZSucceededImpl(HRESULT hr, const wchar_t* pszFile, int line, const wchar_t* pszModule)
    {
        bool bSucceeded = SUCCEEDED(hr);
        ZAssertImpl(bSucceeded, L"Function Failed", pszFile, line, pszModule);
        return bSucceeded;
    }


    #ifdef _TRACE
        bool    g_bEnableTrace = false;
        ZString g_strSpaces;
        int g_indent = 0;
        int g_line   = 0;

        void SetStrSpaces()
        {
            g_strSpaces = 
                  " "
                + ZString((float)g_indent, 2, 0) 
                + ZString(' ', g_indent * 2 + 1);
        }

        void ZTraceImpl(const char* pcc)
        {
            if (g_bEnableTrace) {
                ZDebugOutput(ZString((float)g_line, 4, 0) + g_strSpaces + ZString(pcc) + "\n");
            }
            g_line++;
        }

        void ZEnterImpl(const char* pcc) 
        {
            ZTraceImpl("enter " + ZString(pcc));
            g_indent += 1;
            SetStrSpaces();
        }

        void ZExitImpl(const char* pcc)
        {
            g_indent -= 1;
            SetStrSpaces();
            ZTraceImpl("exit   " + ZString(pcc));
        }

        void ZStartTraceImpl(const char* pcc)
        {
            g_indent = 0;
            g_line   = 0;
            SetStrSpaces();
            ZTraceImpl(pcc);
        }
    #endif

		void debugf(const wchar_t* format, ...)
    {
        if (g_bOutput)
        {
            const size_t size = 2048; //Avalanche: Changed to handle longer messages (from 512)
			wchar_t         bfr[size];

            va_list vl;
            va_start(vl, format);
			_vsnwprintf_s(bfr, size, (size - 1), format, vl); //Avalanche: Fix off by one error. 
            va_end(vl);

            ZDebugOutputImpl(bfr);
        }
    }

    void InitializeDebugf()
    {
        HKEY hKey;
        DWORD dwType;
        wchar_t  szValue[20];
        DWORD cbValue = sizeof(szValue);
        bool  bLogToFile = false;

		// mmf added this regkey check - why arent these dwords? 10/14
        if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey))
        {
            ::RegQueryValueEx(hKey, L"OutputDebugString", NULL, &dwType, (unsigned char*)&szValue, &cbValue);
            ::RegCloseKey(hKey);

            g_outputdebugstring = (wcscmp(szValue, L"1") == 0);
        }

        if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey))
        {
            ::RegQueryValueEx(hKey, L"LogToFile", NULL, &dwType, (unsigned char*)&szValue, &cbValue);
            ::RegCloseKey(hKey);

            bLogToFile = (wcscmp(szValue, L"1") == 0);
        }

        if (bLogToFile)
        {
            time_t  longTime;
            time(&longTime);
			tm* t = new tm;
//            tm*             t = localtime(&longTime);
			localtime_s(t, &longTime);

			wchar_t    logFileName[MAX_PATH + 16];
            GetModuleFileName(NULL, logFileName, MAX_PATH);
			wchar_t*   p = wcsrchr(logFileName, '\\');
            if (!p)
                p = logFileName;
            else
                p++;

			const wchar_t* months[] = { L"jan", L"feb", L"mar", L"apr",
                                    L"may", L"jun", L"jul", L"aug",
                                    L"sep", L"oct", L"nov", L"dec"};
//            strcpy_s(p, _MAX_PATH + 16, months[t->tm_mon]);
//            sprintf_s(p + 3, _MAX_PATH + 13, "%02d%02d%02d%02d.txt",
//                    t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			wcscpy(p, months[t->tm_mon]);
			swprintf(p+3, L"%02d%02d%02d%02d.txt",
				t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			delete t;
			// mmf this is NOT where the logfile AllSrv.txt is generated
			//     this is the client logfile and only for FZDebug build
            g_logfile = 
                CreateFile(
                    logFileName, 
                    GENERIC_WRITE, 
                    FILE_SHARE_READ,
                    NULL, 
                    OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
                    NULL
                );
        }
    }

    void TerminateDebugf()
    {
        if (g_logfile) {
            CloseHandle(g_logfile);
            g_logfile = NULL;
        }
    }
#endif  // SRVLOG or _DEBUG

//////////////////////////////////////////////////////////////////////////////
//
// Win32 Application
//
//////////////////////////////////////////////////////////////////////////////

Win32App::Win32App()
{
    g_papp = this;
}

Win32App::~Win32App() 
{
}

HRESULT Win32App::Initialize(const ZString& strCommandLine)
{
    return S_OK;
}

void Win32App::Terminate()
{
}

void Win32App::Exit(int value)
{
    _CrtSetDbgFlag(0);
    _exit(value);
}

int Win32App::OnException(DWORD code, EXCEPTION_POINTERS* pdata)
{
			GenerateDump(pdata);
	return EXCEPTION_CONTINUE_SEARCH;
}
//Imago 6/10
LONG __stdcall Win32App::ExceptionHandler( EXCEPTION_POINTERS* pep ) 
{
			GenerateDump(pep);
	
	return EXCEPTION_EXECUTE_HANDLER; 
}



#ifdef MemoryOutput
    TList<ZString> g_listOutput;
#endif

	void Win32App::DebugOutput(const wchar_t *psz)
{
    #ifdef MemoryOutput
        g_listOutput.PushFront(ZString(psz));

        if (g_listOutput.GetCount() > 100) {
            g_listOutput.PopEnd();
        }
    #else
		// mmf for now tie this to a registry key
		if (g_outputdebugstring)
			::OutputDebugString(psz);

        if (g_logfile) {
            DWORD nBytes;
            ::WriteFile(g_logfile, psz, wcslen(psz), &nBytes, NULL);
        }
    #endif
}

bool Win32App::OnAssert(const wchar_t* psz, const wchar_t* pszFile, int line, const wchar_t* pszModule)
{
    ZDebugOutput(
          ZString(L"assertion failed: '")
        + psz
		+ L"' ("
        + pszFile
		+ L":"
        + ZString(line)
		+ L")\n"
    );

    //return _CrtDbgReport(_CRT_ASSERT, pszFile, line, pszModule, psz) == 1;
    return true;
}

void Win32App::OnAssertBreak()
{
    #ifdef MemoryOutput
        ZString str;

        TList<ZString>::Iterator iter(g_listOutput);

        while (!iter.End()) {
            str = iter.Value() + str;
            iter.Next();
        }
    #endif
    
    //
    // Cause an exception
    //
	// Imago integrated with mini dump on 6/10
	__try {
		(*(int*)0) = 0;
	}
	__except(GenerateDump(GetExceptionInformation())) {}
}

// KGJV - added for DX9 behavior - default is false. override in parent to change this
bool Win32App::IsBuildDX9()
{
	return false;
}


bool Win32App::WriteMemory( BYTE* pTarget, const BYTE* pSource, DWORD Size )
{
	DWORD ErrCode = 0;


	// Check parameters 

	if( pTarget == 0 )
	{
		_ASSERTE( !_T("Target address is null.") );
		return false;
	}

	if( pSource == 0 )
	{
		_ASSERTE( !_T("Source address is null.") );
		return false;
	}

	if( Size == 0 )
	{
		_ASSERTE( !_T("Source size is null.") );
		return false;
	}

	if( IsBadReadPtr( pSource, Size ) )
	{
		_ASSERTE( !_T("Source is unreadable.") );
		return false;
	}


	// Modify protection attributes of the target memory page 

	DWORD OldProtect = 0;

	if( !VirtualProtect( pTarget, Size, PAGE_EXECUTE_READWRITE, &OldProtect ) )
	{
		ErrCode = GetLastError();
		_ASSERTE( !_T("VirtualProtect() failed.") );
		return false;
	}


	// Write memory 

	memcpy( pTarget, pSource, Size );


	// Restore memory protection attributes of the target memory page 

	DWORD Temp = 0;

	if( !VirtualProtect( pTarget, Size, OldProtect, &Temp ) )
	{
		ErrCode = GetLastError();
		_ASSERTE( !_T("VirtualProtect() failed.") );
		return false;
	}


	// Success 

	return true;

}




bool Win32App::EnforceFilter( bool bEnforce )
{
	DWORD ErrCode = 0;

	
	// Obtain the address of SetUnhandledExceptionFilter 

	HMODULE hLib = GetModuleHandle( _T("kernel32.dll") );

	if( hLib == NULL )
	{
		ErrCode = GetLastError();
		_ASSERTE( !_T("GetModuleHandle(kernel32.dll) failed.") );
		return false;
	}

	BYTE* pTarget = (BYTE*)GetProcAddress( hLib, "SetUnhandledExceptionFilter" );

	if( pTarget == 0 )
	{
		ErrCode = GetLastError();
		_ASSERTE( !_T("GetProcAddress(SetUnhandledExceptionFilter) failed.") );
		return false;
	}

	if( IsBadReadPtr( pTarget, sizeof(OriginalBytes) ) )
	{
		_ASSERTE( !_T("Target is unreadable.") );
		return false;
	}


	if( bEnforce )
	{
		// Save the original contents of SetUnhandledExceptionFilter 

		memcpy( OriginalBytes, pTarget, sizeof(OriginalBytes) );


		// Patch SetUnhandledExceptionFilter 

		if( !WriteMemory( pTarget, PatchBytes, sizeof(PatchBytes) ) )
			return false;

	}
	else
	{
		// Restore the original behavior of SetUnhandledExceptionFilter 

		if( !WriteMemory( pTarget, OriginalBytes, sizeof(OriginalBytes) ) )
			return false;

	}


	// Success 

	return true;

}

//////////////////////////////////////////////////////////////////////////////
//
// Win Main
//
//////////////////////////////////////////////////////////////////////////////

__declspec(dllexport) int WINAPI Win32Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmdLine, int nCmdShow)
{
    HRESULT hr;

    // seed the random number generator with the current time
    // (GetTickCount may be semi-predictable on server startup, so we add the 
    // clock time to shake things up a bit)
    srand(GetTickCount() + (int)time(NULL));

	// mmf why is this done?
    // shift the stack locals and the heap by a random amount.            
    char* pzSpacer = new char[4 * (int)random(21, 256)];
    pzSpacer[0] = *(char*)_alloca(4 * (int)random(1, 256));

	//Imago 6/10
	SetUnhandledExceptionFilter(Win32App::ExceptionHandler); 
	g_papp->EnforceFilter( true );

    __try { 
        do {
            #ifdef SRVLOG
                InitializeDebugf();
            #endif

			InitializeLogchat();  // mmf

            BreakOnError(hr = Window::StaticInitialize());

// BUILD_DX9 - KGJV use runtime dynamic instead at preprocessor level
			if (g_papp->IsBuildDX9())
			{
				BreakOnError(hr = g_papp->Initialize(lpszCmdLine));
			}
			else
			{
				// Don't throw an error, if the user selects cancel it can return E_FAIL.
				hr = g_papp->Initialize(lpszCmdLine);
			}
// BUILD_DX9

            //
            // Win32App::Initialize() return S_FALSE if this is a command line app and
            // we shouldn't run the message loop
            //

            if (SUCCEEDED(hr) && S_FALSE != hr) {
                Window::MessageLoop();
            }

            g_papp->Terminate();
            Window::StaticTerminate();

            #ifdef SRVLOG
                TerminateDebugf();
            #endif

			TerminateLogchat(); // mmf

        } while (false);
    }  __except (g_papp->OnException(_exception_code(), (EXCEPTION_POINTERS*)_exception_info())){
    }  
    delete pzSpacer;
	if( !g_papp->EnforceFilter( false ) )
	{
		debugf(L"EnforceFilter(false) failed.\n");
		return 0;
	}
    return 0;
}
