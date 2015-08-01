/*-------------------------------------------------------------------------
  AutoUpdate.cpp
  
  Management of the AutoUpdate system
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/


#include "pch.h"



class CAutoUpdateImpl :
    public IAutoUpdate
{

public:

    CAutoUpdateImpl() :
      m_crcFileList(0),
      m_nFileListSize(0),
	  m_ttLastFileCheck(0) // BT - 7/15 - Enable the lobby to pick up FileList.txt changes without restarting.
    {
    }


    /////////////////////////////////////////////////////////////////////////

    void    LoadSettings(HKEY hk)
    {
        bool m_bAutoUpdateActive;

        m_bAutoUpdateActive = _Module.ReadFromRegistry(hk, true, "FTPArtServer", m_szFTPServer, NULL);
        m_bAutoUpdateActive = _Module.ReadFromRegistry(hk, true, "FTPArtRoot", m_szFTPInitialDir, NULL) && m_bAutoUpdateActive;
        _Module.ReadFromRegistry(hk, true, "FTPAccount", m_szFTPAccount, (DWORD)"Anonymous", false);
        _Module.ReadFromRegistry(hk, true, "FTPPW", m_szFTPPassword, (DWORD)"Unknown", false);

        if (!m_bAutoUpdateActive)
        {
          _Module.LogEvent(EVENTLOG_ERROR_TYPE, LE_BadAutoUpdateConfig);
          g_pAutoUpdate = NULL;
          delete this;
        }
    }

    /////////////////////////////////////////////////////////////////////////

    void   LoadCRC(char * szFileName) 
    {
        char szErrorMsg[200+MAX_PATH];

		// BT - 7/15 - Enable the lobby to pick up FileList.txt changes without restarting.
		strcpy(m_szFileListPath, szFileName);
		m_ttLastFileCheck = time(0);

        HANDLE hFile = CreateFileA(szFileName, 
                                  GENERIC_READ, 
                                  FILE_SHARE_READ, 
                                  NULL, 
                                  OPEN_EXISTING, 
                                  FILE_ATTRIBUTE_NORMAL, 
                                  NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
           sprintf(szErrorMsg, "Failed open file (%s); Error Code: %d ", szFileName, GetLastError());
        }
        else
        {
            m_nFileListSize = GetFileSize(hFile, NULL);

            char szErrorMsgBuffer[100+MAX_PATH];

            m_crcFileList = FileCRC(hFile, szErrorMsgBuffer);

            if (m_crcFileList == 0)
            {
                sprintf(szErrorMsg, "Error while determining CRC; %s", szErrorMsgBuffer);
            }
            else
            {
                ::CloseHandle(hFile);
                return;
            }
    
        }

        _Module.LogEvent(EVENTLOG_ERROR_TYPE, LE_AutoUpdateDeactivated, szErrorMsg);
        g_pAutoUpdate = NULL;
        delete this;
    }

    /////////////////////////////////////////////////////////////////////////

    char *  GetFTPServer()
    {
        return m_szFTPServer;
    }

    /////////////////////////////////////////////////////////////////////////

    char *  GetFTPInitialDir()
    {
        return m_szFTPInitialDir;
    }

    /////////////////////////////////////////////////////////////////////////

    char *  GetFTPAccount()
    {
        return m_szFTPAccount;
    }

    /////////////////////////////////////////////////////////////////////////

    char *  GetFTPPassword()    
    {
        return m_szFTPPassword;
    }

	// BT - 7/15 - Enable the lobby to pick up FileList.txt changes without restarting.
	// Allow the lobby to reload FileList.txt if it is modified without requireing a lobby restart.
	void ReloadFileListIfRequired()
	{
		time_t currentTime = time(0);

		if (currentTime - m_ttLastFileCheck > 30)
		{
			debugf("Reloading %s\r\n", m_szFileListPath);
			LoadCRC(m_szFileListPath);
		}
	}

    /////////////////////////////////////////////////////////////////////////

    int     GetFileListCRC()    
    {
		// BT - 7/15 - Enable the lobby to pick up FileList.txt changes without restarting.
		ReloadFileListIfRequired();

        return m_crcFileList;
    }

    /////////////////////////////////////////////////////////////////////////

    unsigned GetFileListSize()
    {
		// BT - 7/15 - Enable the lobby to pick up FileList.txt changes without restarting.
		ReloadFileListIfRequired();

        return m_nFileListSize;
    }

    /////////////////////////////////////////////////////////////////////////

private:

  char              m_szFTPServer[MAX_PATH+1];
  char              m_szFTPInitialDir[MAX_PATH+1];
  char              m_szFTPAccount[MAX_PATH+1];
  char              m_szFTPPassword[MAX_PATH+1];
  int               m_crcFileList;
  unsigned          m_nFileListSize;
  char				m_szFileListPath[MAX_PATH + 1]; // BT - 7/15 - Enable the lobby to pick up FileList.txt changes without restarting.
  time_t			m_ttLastFileCheck; // BT - 7/15 - Enable the lobby to pick up FileList.txt changes without restarting.
};

IAutoUpdate * g_pAutoUpdate = NULL; // NULL if AutoUpdate is disabled


void CreateAutoUpdate(HKEY hk, char * szFileName)
{
    g_pAutoUpdate = new CAutoUpdateImpl();

    g_pAutoUpdate->LoadSettings(hk);

    if (g_pAutoUpdate) // if loading settings failed, g_pAutoUpdate is NULL
    {
        g_pAutoUpdate->LoadCRC(szFileName);
    }
}
