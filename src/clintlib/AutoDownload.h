
/*-------------------------------------------------------------------------
 * clintlib\AutoDownload.h
 *
 *
 * Owner:
 *
 * Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "regkey.h"

class IAutoUpdateSink;
/*      AutoDownload incepts the logon ack, delaying it until the download
 *      is complete.  Use this to tell Autodown what to Ack with, once the
 *      transfer is complete.
 */





class IAutoDownload
{
public:

    /*-------------------------------------------------------------------------
     * SetFTPSite()
     *-------------------------------------------------------------------------
     * Purpose:
     *      Set the site info so we know what sever to connect to.
     */
	virtual void SetFTPSite(const wchar_t * szFTPSite, const wchar_t * szInitialDirectory, const wchar_t * szUsername, const wchar_t * szPassword) = 0;

    /*-------------------------------------------------------------------------
     * SetOfficialFileListAttributes()
     *-------------------------------------------------------------------------
     * Purpose:
     *      Set the CRC we can verify that the FTP site has the correct FileList.txt
     *
     * Parameters:
     *      nCRC: official CRC for FileList.txt at FTP site
     *      nFileSize: size of FileList.txt at FTP Site
     */
    virtual void SetOfficialFileListAttributes(int nCRC, unsigned nFileSize) = 0;

    /*-------------------------------------------------------------------------
     * SetOfficialTime()
     *-------------------------------------------------------------------------
     */
	virtual void SetArtPath(const wchar_t * pArtPath) = 0;

    /*-------------------------------------------------------------------------
     * SetFilelistSubDir()
     * Since the filename FileList.txt is hardcoded everywhere, and sometimes
     * we want differentiate between two filelists, we have this.
     * This is the sub dir of where the filelist is downloaded from; if not
     * set, then the regular base directory is used.
     *-------------------------------------------------------------------------
     */
	virtual void SetFilelistSubDir(const wchar_t * pszPath) = 0;

    /*-------------------------------------------------------------------------
     * BeginUpdate()
     *-------------------------------------------------------------------------
     * Purpose:
     *      Setup variables and download the file list.
     *
     * Parameters:
     *      pSink: pointer to the object receiving events about the download
     *      bForceCRCCheck: if true, file times are ignored and CRC are always checked
     *      bSkipReloader: if true, then reloader is not launch: an error is displayed
     */
    virtual void BeginUpdate(IAutoUpdateSink * pSink, bool bForceCRCCheck, bool bSkipReloader) = 0;

    /*-------------------------------------------------------------------------
     * HandleAutoDownload()
     *-------------------------------------------------------------------------
     * Parameters:
     *      dwTimeAlloted: time slice that we have to get or process data.
     *                     HandleAutoDownload() will usually take AT LEAST this much
     *                     time, and on rare occasions less.
     * Purpose:
     *      Check to see if AutoUpdate is done from within the main thread.  If so,
     *    restart or relogon as needed.
     *
     * Remarks:
     *      This is intended to be called periodically during the download.
     */
     virtual void HandleAutoDownload(DWORD dwTimeAlloted) = 0;

    /*-------------------------------------------------------------------------
     * Abort()
     *-------------------------------------------------------------------------
     * Purpose:
     *      Abort the autodownload.
     */
     virtual void Abort() = 0;
};


IAutoDownload * CreateAutoDownload();






/*---------------------------------------------------------------------------------

  IAutoUpdateSink:

  This receives events about the Autodownload system

 ---------------------------------------------------------------------------------*/

class IAutoUpdateSink :
    public IHTTPSessionSink
{
public:
    virtual void OnBeginRetrievingFileList() {}

    virtual void OnRetrievingFileListProgress(unsigned long nFileSize, unsigned long cCurrentBytes) {}

    // once we've got the right filelist, we begin analysis of local files
    virtual void OnBeginAnalysis() {}

    virtual void OnAnalysisProgress(float fPercentDone) {}

	virtual bool ShouldFilterFile(const wchar_t * szFileName) // szFileName is base filename (not including path)
    {
      return false; // if returns true, then file is not downloaded
    }

	virtual void OnProgress(unsigned long cTotalBytes, const wchar_t* szCurrentFile, unsigned long cCurrentFileBytes, unsigned cEstimatedSecondsLeft) {}

    virtual void OnBeginDownloadProgressBar(unsigned cTotalBytes, int cFiles) {}

    virtual void OnUserAbort() {}

    /*-------------------------------------------------------------------------
     * SetErrorFunction()
     *-------------------------------------------------------------------------
     * Purpose:
     *      Set a callback function for when an error occurs during moving files.
     *
     * Returns: true if the autoupdate system should try again
     */
    virtual bool OnMoveError(wchar_t * szErrorMsg)
    {
      return false;
    }

    // returns true if file should be registered
	virtual bool ShouldRegister(wchar_t * szFullFileName) // path is included in szFullFileName
    {
      return false;
    }

    // returns registration exit code (0 means success)
	virtual int RegisterFile(wchar_t * szFullFileName) // path is included in szFullFileName
    {
      return -1;
    }

    // this is called in the AutoUpdate system destructor
    virtual void OnAutoUpdateSystemTermination(bool bErrorOccurred, bool bRestarting) {}
};







class CAutoDownloadUtil  // Shared between Reloader.exe and IAutoDownload's privates
{
public:

    /*-------------------------------------------------------------------------
     * MoveFiles()
     *-------------------------------------------------------------------------
     * Purpose:
     *      Copies temp files into ArtPath.  There's a hardcoded check for
     *      Allegiance.exe. Assumes current folder is where Allegiance.exe
     *      should go (not ArtPath).
     *
     * Paramters:
     *      szTempPath:  where the all the files come from
     *      szArtPath:   where the art files go
     *      bSkipSharingViolation: if true then we don't quit when a sharing violation occurs.
     *                             if false then we quit with error on such cases.
     *      pbFilesWereSkipped: is used at only if bSkipSharingViolation is true -AND- pbFilesWereSkipped != NULL.
     *                          *pbFilesWereSkipped is set to true if files were indeed skipped due to a
     *                          sharing violation.
     *      bNoRegistryWrite: do not update registry
     *      pSink : AutoUpdate sink where move errors are reported
     * Returns:
     *      true only on success
     */
	static bool MoveFiles(const wchar_t * szTempPath, const wchar_t * szArtPath_, bool bSkipSharingViolation,
		bool * pbFilesWereSkipped, bool bNoRegistryWrite, wchar_t * szErrorMsg, IAutoUpdateSink * pSink)
    {

      WIN32_FIND_DATAW finddata;
      HANDLE hsearchFiles = 0;

	  wchar_t szSourceSpec[MAX_PATH + 20];
	  wchar_t szArtPath[MAX_PATH + 20];
      Strcpy(szSourceSpec, szTempPath);
      Strcat(szSourceSpec, L"*.*");

      Strcpy(szArtPath, szArtPath_);

      int cLen = wcslen(szArtPath);
      if (cLen == 0 || szArtPath[cLen-1] != '\\')
      {
          szArtPath[cLen++] = '\\';
          szArtPath[cLen] = 0;
      }

      bool bFilesWereSkipped = false;

      // count the files in the file path
      hsearchFiles = FindFirstFileW(szSourceSpec, &finddata);
      if (INVALID_HANDLE_VALUE == hsearchFiles)
      {
          return false;
      }

	  wchar_t szSource[MAX_PATH + 20];
	  wchar_t szDest[MAX_PATH + 20];

      while (INVALID_HANDLE_VALUE != hsearchFiles)
      {
        // skip directory listings "." and ".."; and filelist (filelist is moved last)
        if (finddata.cFileName[0] != '.' &&
            _wcsicmp(finddata.cFileName, L"FileList.txt") != 0 &&
            (!pSink || !pSink->ShouldFilterFile(finddata.cFileName)))
        {
            // setup move paths
            Strcpy(szSource, szTempPath);
            Strcat(szSource, finddata.cFileName);
            GetFileNameWithPath(szDest, finddata.cFileName, szArtPath, L".\\");

            // Move files to their dest
            if (!MoveFilePrivate(szSource, szDest, bSkipSharingViolation, &bFilesWereSkipped, szErrorMsg, pSink))
                return false;

            // consider registering special files
            if(pSink && pSink->ShouldRegister(szDest))
            {
              int nExitCode = pSink->RegisterFile(szDest);

              if (nExitCode != 0)
              {
                // registration failed; move file back so autoupdate attempts to register later; then abort
                MoveFile(szDest, szSource);

                if (szErrorMsg)
                    swprintf(szErrorMsg, L"Failed to Register file %s; registration exit code(%d)", finddata.cFileName, nExitCode);

                return false;
              }
            }
        }

        if (!FindNextFile(hsearchFiles, &finddata))
        {
          FindClose(hsearchFiles);
          hsearchFiles = INVALID_HANDLE_VALUE;
        }
      }

      //
      // finish off with moving the filelist; if no files were skipped; if files were skipped, then reloader
      // needs to move the filelist
      //
      if(!bFilesWereSkipped)
      {
          Strcpy(szSource, szTempPath);
          Strcat(szSource, L"FileList.txt");

          if (CAutoDownloadUtil::GetFileLength(szSource) != -1) // check for existance
          {
            GetFileNameWithPath(szDest, L"FileList.txt", szArtPath, L".\\");

            if (!MoveFilePrivate(szSource, szDest, bSkipSharingViolation, &bFilesWereSkipped, szErrorMsg, pSink))
                return false;
          }
      }

      FindClose(hsearchFiles);

      if (bSkipSharingViolation && pbFilesWereSkipped)
      *pbFilesWereSkipped = bFilesWereSkipped;

      if (!bFilesWereSkipped && !bNoRegistryWrite)
      {
          // Set registry's MoveInProgress to zero, meaning move is complete
          HKEY hKey;
          DWORD dwValue = 0;
          if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_WRITE, &hKey))
          {
            ::RegSetValueEx(hKey, L"MoveInProgress", NULL, REG_DWORD, (unsigned char*)&dwValue, sizeof(DWORD));
          }
      }

      return true;
    }

    static wchar_t * GetEXEFileName(int nIndex)
    {
		static wchar_t * pszEXEFiles[] =
        {
            L"CliConfig.exe",
			L"fsmon.exe",
			L"readme.txt",
			L"patcher.exe",
			L"FileList.txt",
			L"Reloader.exe",
			L"msrgbits.inf",
			L"msrgtran.dll",
			L"msrgip.dll",
			L"dbghelp.dll"
            // the file muse be at least 8 characters (including ext)  For example: fsmon.exe
            // increment g_cEXEFiles, if you add to this
        };
        #define g_cEXEFiles 10 //imago dur 6/10

        return pszEXEFiles[nIndex];
    }

    /*-------------------------------------------------------------------------
     * GetFileNameWithPath()
     *-------------------------------------------------------------------------
     * Returns:
     *      The full path of where the file belongs.
     */
	static void GetFileNameWithPath(OUT wchar_t * szFileNameWithPath,
		IN  const wchar_t * szRawFileName,
		IN  const wchar_t * szArtPath,
		IN  const wchar_t * szEXEPath)
    {
        //
        // Move special files to current directory
        //

        //
        // NOTE: if you add special files, try adding them to the GetEXEFileName()
        // list to ensure get special care for the PC Gamer Build (Beta 1) bug.
        //   ///////////////////////////////////////////////////////
        if (_wcsicmp(szRawFileName, L"AllegianceRetail.exe") == 0 ||
			_wcsicmp(szRawFileName, L"AllegianceDebug.exe") == 0 ||
			_wcsicmp(szRawFileName, L"AllegianceTest.exe") == 0 ||
			_wcsicmp(szRawFileName, L"Allegiance.exe") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
            Strcat(szFileNameWithPath, L"Allegiance.exe");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllegianceRetail.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllegianceDebug.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllegianceTest.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"Allegiance.pdb") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
            Strcat(szFileNameWithPath, L"Allegiance.pdb");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllegianceRetail.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllegianceDebug.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllegianceTest.sym") == 0 ||
				_wcsicmp(szRawFileName, L"Allegiance.sym") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"Allegiance.sym");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllegianceRetail.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllegianceDebug.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllegianceTest.map") == 0 ||
				_wcsicmp(szRawFileName, L"Allegiance.map") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"Allegiance.map");
        }
        else ///////////////////////////////////////////////////////
			if (_wcsicmp(szRawFileName, L"AllSrvRetail.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvDebug.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvTest.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrv.exe") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrv.exe");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllSrvRetail.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvDebug.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvTest.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrv.pdb") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrv.pdb");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllSrvRetail.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvDebug.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvTest.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrv.sym") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrv.sym");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllSrvRetail.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvDebug.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvTest.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrv.map") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrv.map");
        }
        else ///////////////////////////////////////////////////////
			if (_wcsicmp(szRawFileName, L"AllSrvRetail.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvDebug.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvTest.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrv.exe") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrv.exe");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllSrvRetail.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvDebug.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvTest.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrv.pdb") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrv.pdb");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllSrvRetail.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvDebug.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvTest.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrv.sym") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrv.sym");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllSrvRetail.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvDebug.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvTest.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrv.map") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrv.map");
        }
        else ///////////////////////////////////////////////////////
			if (_wcsicmp(szRawFileName, L"AGCRetail.dll") == 0 ||
				_wcsicmp(szRawFileName, L"AGCDebug.dll") == 0 ||
				_wcsicmp(szRawFileName, L"AGCTest.dll") == 0 ||
				_wcsicmp(szRawFileName, L"AGC.dll") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AGC.dll");
        }
        else
			if (_wcsicmp(szRawFileName, L"AGCRetail.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AGCDebug.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AGCTest.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AGC.pdb") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AGC.pdb");
        }
        else
			if (_wcsicmp(szRawFileName, L"AGCRetail.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AGCDebug.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AGCTest.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AGC.sym") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AGC.sym");
        }
        else
			if (_wcsicmp(szRawFileName, L"AGCRetail.map") == 0 ||
				_wcsicmp(szRawFileName, L"AGCDebug.map") == 0 ||
				_wcsicmp(szRawFileName, L"AGCTest.map") == 0 ||
				_wcsicmp(szRawFileName, L"AGC.map") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AGC.map");
        }
        else ///////////////////////////////////////////////////////
			if (_wcsicmp(szRawFileName, L"AllSrvUIRetail.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUIDebug.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUITest.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUI.exe") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrvUI.exe");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllSrvUIRetail.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUIDebug.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUITest.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUI.pdb") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrvUI.pdb");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllSrvUIRetail.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUIDebug.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUITest.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUI.sym") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrvUI.sym");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllSrvUIRetail.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUIDebug.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUITest.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUI.map") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrvUI.map");
        }
        else ///////////////////////////////////////////////////////
			if (_wcsicmp(szRawFileName, L"AllSrvUIRetail.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUIDebug.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUITest.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUI.exe") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrvUI.exe");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllSrvUIRetail.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUIDebug.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUITest.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUI.pdb") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrvUI.pdb");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllSrvUIRetail.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUIDebug.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUITest.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUI.sym") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrvUI.sym");
        }
        else
			if (_wcsicmp(szRawFileName, L"AllSrvUIRetail.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUIDebug.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUITest.map") == 0 ||
				_wcsicmp(szRawFileName, L"AllSrvUI.map") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AllSrvUI.map");
        }
        else ///////////////////////////////////////////////////////
			if (_wcsicmp(szRawFileName, L"AutoUpdateRetail.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AutoUpdateDebug.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AutoUpdateTest.exe") == 0 ||
				_wcsicmp(szRawFileName, L"AutoUpdate.exe") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AutoUpdate.exe");
        }
        else
			if (_wcsicmp(szRawFileName, L"AutoUpdateRetail.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AutoUpdateDebug.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AutoUpdateTest.pdb") == 0 ||
				_wcsicmp(szRawFileName, L"AutoUpdate.pdb") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AutoUpdate.pdb");
        }
        else
			if (_wcsicmp(szRawFileName, L"AutoUpdateRetail.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AutoUpdateDebug.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AutoUpdateTest.sym") == 0 ||
				_wcsicmp(szRawFileName, L"AutoUpdate.sym") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AutoUpdate.sym");
        }
        else
			if (_wcsicmp(szRawFileName, L"AutoUpdateRetail.map") == 0 ||
				_wcsicmp(szRawFileName, L"AutoUpdateDebug.map") == 0 ||
				_wcsicmp(szRawFileName, L"AutoUpdateTest.map") == 0 ||
				_wcsicmp(szRawFileName, L"AutoUpdate.map") == 0)
        {
            Strcpy(szFileNameWithPath, szEXEPath);
			Strcat(szFileNameWithPath, L"AutoUpdate.map");
        }
        else
        {
            for (int i = 0; i < g_cEXEFiles; ++i)
            {
				if (_wcsicmp(szRawFileName, GetEXEFileName(i)) == 0)
                {
                    Strcpy(szFileNameWithPath, szEXEPath);
                    Strcat(szFileNameWithPath, szRawFileName);
                    return;
                }
            }
            //
            // Must be an ArtFile!
            //
            Strcpy(szFileNameWithPath, szArtPath);
            Strcat(szFileNameWithPath, szRawFileName);
        }
    }


    //////////////////////////////////////////////////////////////////////////

	static unsigned GetFileLength(wchar_t *szFileName)
    {
        HANDLE hFile = CreateFile(szFileName,
                                 0/*GENERIC_READ*/,  // 0 == query only
                                 FILE_SHARE_READ,
                                 NULL,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            return (unsigned)-1;
        }

        unsigned nSize = ::GetFileSize(hFile, NULL);

        ::CloseHandle(hFile);

        return nSize;
    }

private:


    /*-------------------------------------------------------------------------
     * Move File considering bSkipSharingViolation
     *
     * returns false if error, true on success
     */
	static bool MoveFilePrivate(wchar_t * szSource, wchar_t * szDest, bool bSkipSharingViolation,
		bool * pbFilesWereSkipped, wchar_t * szErrorMsg, IAutoUpdateSink * pSink)
    {
        bool bErrorOccured;
        bool bTryAgain;

        do
        {
            ::DeleteFile(szDest);

            // Note: considered using MoveFileEx, but win95/98 doesn't support it
            BOOL bResult = ::MoveFile(szSource, szDest);

            if(!bResult)
            {
                if (bSkipSharingViolation)
                {
                    int nErr = GetLastError();

                    bool bSkip = (nErr == ERROR_ALREADY_EXISTS || nErr == ERROR_ACCESS_DENIED || nErr == ERROR_SHARING_VIOLATION);

                    if (bSkip)
                    {
                        *pbFilesWereSkipped = true;
                        bErrorOccured = false;
                    }
                    else
                        bErrorOccured = true;
                }
                else
				{
					if (g_outputdebugstring) { //Imago changed from _DEBUG ifdef 8/17/09
	                    wchar_t sz[MAX_PATH];
						swprintf(sz, L"Moving Files Error: %d", GetLastError());
	                    ZDebugOutput(sz);
					}

                    bErrorOccured = true;
                }
            }
            else
                bErrorOccured = false;

            bTryAgain = false;

            if (bErrorOccured && (pSink || szErrorMsg))
            {
                wchar_t szErrorMessage[2*MAX_PATH+50];

                FormatErrorMessage(szErrorMessage, GetLastError());

                Strcat(szErrorMessage, L"\n\r\n\r   Source: ");

				Strcat(szErrorMessage, szSource);

				Strcat(szErrorMessage, L"\n\r\n\r   Dest: ");

				Strcat(szErrorMessage, szDest);

                if (szErrorMsg)
                    Strcpy(szErrorMsg, szErrorMessage);

                if (pSink)
                   bTryAgain = pSink->OnMoveError(szErrorMessage);
            }

        } while(bTryAgain);

        return !bErrorOccured; // true if successful move
    }

    /*-------------------------------------------------------------------------
     * FormatErrorMessage()
     *-------------------------------------------------------------------------
     * Paramters:
     *    dwErrorCode: take a dwErrorCode and print what it means as text
     *
     */
    static void FormatErrorMessage(wchar_t *szBuffer, DWORD dwErrorCode)
    {

      swprintf(szBuffer,L"(%d) ", dwErrorCode);

      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    dwErrorCode,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    szBuffer + wcslen(szBuffer),
                    128,
                    NULL
                    );
    }

};

bool LaunchReloaderAndExit(bool bReLaunchAllegianceAsMinimized);
