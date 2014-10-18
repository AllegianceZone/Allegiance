#include "pch.h"
#include "LogFile.h"


////////////////////////////////////////////////////////////////////////////////
// CLogFile()
// Constructor.
////////////////////////////////////////////////////////////////////////////////
CLogFile::CLogFile(wchar_t * szFileName)
{
	m_hLogFile = INVALID_HANDLE_VALUE;

	wcscpy_s( m_szFileName, MAX_PATH, szFileName );
	GetModuleFileName( NULL, m_szFilePath, MAX_PATH);

	wchar_t * p = wcsrchr(m_szFilePath, '\\');
	if (!p)
		p = m_szFilePath;
	else
		p++;

	wcscpy_s( p, MAX_PATH - ( wcslen( m_szFilePath ) - wcslen( p ) ), szFileName );

	m_hLogFile = CreateFile(	m_szFilePath, 
								GENERIC_WRITE, 
								0,
								NULL, 
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
								NULL );
	_ASSERT( m_hLogFile != INVALID_HANDLE_VALUE );

	OutputStringV( L"Log file %s created - %s\n\n", szFileName, GetTimeStampString() );
}


////////////////////////////////////////////////////////////////////////////////
// ~CLogFile()
// Destructor.
////////////////////////////////////////////////////////////////////////////////
CLogFile::~CLogFile(void)
{
	CloseLogFile();
}


////////////////////////////////////////////////////////////////////////////////
// CloseLogFile()
//
////////////////////////////////////////////////////////////////////////////////
void CLogFile::CloseLogFile()
{
	if( m_hLogFile != INVALID_HANDLE_VALUE )
	{
		OutputString( L"\nLog closed.\n" );
		CloseHandle( m_hLogFile );
		m_hLogFile = INVALID_HANDLE_VALUE;
	}
}


////////////////////////////////////////////////////////////////////////////////
// OutputString()
// Output a standard string to the log file.
////////////////////////////////////////////////////////////////////////////////
void CLogFile::OutputString(wchar_t * szString)
{
	if( m_hLogFile != INVALID_HANDLE_VALUE )
	{
		DWORD nBytes;
		::WriteFile(m_hLogFile, szString, wcslen(szString), &nBytes, NULL);
	}
	debugf(szString);
}


////////////////////////////////////////////////////////////////////////////////
// LogString()
//
////////////////////////////////////////////////////////////////////////////////
void CLogFile::OutputStringV(wchar_t * szFormat, ...)
{
	if( m_hLogFile != INVALID_HANDLE_VALUE )
	{
		const size_t size = 2048;			// Based on debugf value.
		wchar_t szTemp[size];
		va_list vl;
		va_start( vl, szFormat );
		_vsnwprintf_s( szTemp, size, (size-1), szFormat, vl );
		va_end(vl);
		OutputString( szTemp );
	}
}


////////////////////////////////////////////////////////////////////////////////
// GetTimeStampString()
// Returns the output buffer as a string for use as a input parameter.
////////////////////////////////////////////////////////////////////////////////
wchar_t * CLogFile::GetTimeStampString()
{
	time_t longTime;
	tm t;

	time(&longTime);
	localtime_s( &t, &longTime);

	const wchar_t * months[] = { L"Jan", L"Feb", L"Mar", L"Apr",
		L"May", L"Jun", L"Jul", L"Aug",
		L"Sep", L"Oct", L"Nov", L"Dec" };
	wcscpy_s( m_szTimeStamp, 256, months[ t.tm_mon ] );
	swprintf_s( m_szTimeStamp+3, 256 - 3, L" %02d, %02d:%02d", t.tm_mday, t.tm_hour, t.tm_min );

	return m_szTimeStamp;
}
