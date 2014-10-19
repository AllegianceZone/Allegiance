#pragma once

class CLogFile
{
private:
	HANDLE m_hLogFile;
	wchar_t m_szFilePath[MAX_PATH];
	wchar_t m_szFileName[MAX_PATH];
	wchar_t m_szTimeStamp[256];

public:
	CLogFile(wchar_t * szLogName);
	~CLogFile(void);

	void CloseLogFile();
	void OutputString(wchar_t * szString);
	void OutputStringV(wchar_t * szFormat, ...);
	wchar_t * GetTimeStampString();
};
