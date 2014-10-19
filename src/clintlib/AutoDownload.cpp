/*-------------------------------------------------------------------------
 * clintlib\AutoDownload.cpp
 *
 * The implementation of IAutoDownload
 *
 * See AutoDownload.h for descriptions of all public functions.
 *
 * Owner:
 *
 * Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/


#include "pch.h"


// forward local functions
class CAutoDownloadImpl;
class CLocalFilesVerifier;
CLocalFilesVerifier * CreateLocalFilesVerifier(CAutoDownloadImpl * p, bool bForceCRCCheck, int nOfficialCRC, wchar_t * szArtPath);
void DestroyFilesVerifier(CLocalFilesVerifier * p);
bool ContinueLocalFilesVerification(CLocalFilesVerifier * p, DWORD dwTimeAlloted);

void SetLocalFileTime(HANDLE hFile, wchar_t *szFileName, SYSTEMTIME * psystime);

void _debugf(const wchar_t* format, ...);

/*

  Optimization ideas:

     Start downloading while we are still building filelist.  This would allow for bottom two
     progress bars to move at the same time.

  */


class CAutoDownloadImpl:
  public IAutoDownload,
  public IHTTPSessionSink
{

public:

    struct CFileInfo
    {
        int         nCRC;
        int         nSize;
        SYSTEMTIME  stTime;
    };


    CAutoDownloadImpl():
          m_pHTTPSession(NULL),
          m_pLocalFilesVerifier(NULL),
          m_cFiles(0),
          m_bErrorHasOccurred(false),
          m_bReadmeUpdated(false),
          m_cBytesCompressionSavings(0),
          m_cHTTPServerFails(0),
          m_cTotalBytes(0)
    {
      m_szErrorMessage[0] = 0;
  	  Strcpy(m_szURL, L"FTPServer Key (in Server's Registry) is probably pointing to an invalid FTP/HTTP Site");
      m_cListAllocSize = 8;
	  m_pszFileList = (wchar_t**)realloc(NULL, m_cListAllocSize * sizeof(wchar_t*));
      *m_pszFileList = NULL;
      m_pFileInfo = (CFileInfo*)realloc(NULL, m_cListAllocSize * sizeof(CFileInfo));
      m_szFilelistSubDir[0] = '\0';

	  wchar_t    path[MAX_PATH + 16];
      ::GetModuleFileName(NULL, path, MAX_PATH);
	  wchar_t*   p = wcsrchr(path, '\\');
      if (p)
      {
          p++;
          *p = 0; // erase filename
          Strcpy(m_szEXEPath, path);
      }
      else
      {
          m_szEXEPath[0] = '\\';
          m_szEXEPath[1] = '\0';
      }
   }

    ~CAutoDownloadImpl()
    {
        m_pSink->OnAutoUpdateSystemTermination(m_bErrorHasOccurred, m_bNeedToRestart);

        if (m_pszFileList)
        {
            for (unsigned i = 0; i < m_cFiles; ++i)
            {
                free(m_pszFileList[i]);
            }

            free(m_pszFileList);
        }

        if (m_pFileInfo)
            free(m_pFileInfo);

        if (m_pHTTPSession)
            delete m_pHTTPSession;

        if(m_pLocalFilesVerifier)
            DestroyFilesVerifier(m_pLocalFilesVerifier);
    }


public:

    ///////////////////////////////////////////////////////////////////////////////////////

	void SetFTPSite(const wchar_t * szFTPSite, const wchar_t * szInitialDirectory, const wchar_t * szUsername, const wchar_t * szPassword)
    {
        //
        // Prepare m_szURL
        //
		if (szFTPSite)
        {
            //
            //  figure out FTP/HTTP
            //
            if (_wcsnicmp(szFTPSite, L"ftp://", 6) == 0)
            {

                /* TODO: make Username and Password work for FTP
                if (szUsername && szUsername[0] != 0 && szPassword && szPassword[0] != 0)
                {
                }
                else
                */
                    Strcpy(m_szURL, szFTPSite);


            }
            else // http
            {
                if (_wcsnicmp(szFTPSite, L"http://", 7) == 0)
                {
                    Strcpy(m_szURL, szFTPSite);
                }
                else
                {
                    Strcpy(m_szURL, L"http://");
                    Strcat(m_szURL, szFTPSite);
                }
            }

            if (szInitialDirectory)
            {
                // ensure just one slash seperator
                if (m_szURL[0] != '\0' && m_szURL[wcslen(m_szURL)-1] != '/' && szInitialDirectory[0] != '/')
                    Strcat(m_szURL, L"/");

                Strcat(m_szURL, szInitialDirectory);
            }

            // ensure last character is a slash
            if (m_szURL[0] != '\0' && m_szURL[wcslen(m_szURL)-1] != '/')
                Strcat(m_szURL, L"/");
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    // Set the CRC we can verify that the FTP site has the correct FileList.txt
    void SetOfficialFileListAttributes(int nCRC, unsigned nFileSize)
    {
        m_nOfficialCRC = nCRC;
        m_nFileListSize = nFileSize;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

	void SetArtPath(const wchar_t * szArtPath)
    {
        //
        // Make sure art path is in a friendly format as we copy it
        //

        int cLen = wcslen(szArtPath);
        int i,j;
        for (i = 0, j = 0; i < cLen; ++i)
            if (szArtPath[i] == '/')
                m_szArtPath[j++] = '\\';
            else
            {
              if (szArtPath[i] != '\"')
                  m_szArtPath[j++] = szArtPath[i];
            }

        if (m_szArtPath[j] != '\\')
        {
            m_szArtPath[j++] = '\\';
            m_szArtPath[j] = '\0';
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////

	void SetFilelistSubDir(const wchar_t * pszPath)
    {
        Strncpy(m_szFilelistSubDir, pszPath, sizeof(m_szFilelistSubDir));
        // ensure last character is a slash
        if (m_szFilelistSubDir[0] != '\0' && m_szFilelistSubDir[wcslen(m_szFilelistSubDir)-1] != '/')
            Strcat(m_szFilelistSubDir, L"/");
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    void BeginUpdate(IAutoUpdateSink * pSink, bool bForceCRCCheck, bool bSkipReloader)
    {
        //
        // Make sure the current path is where Allegiance.exe is for the AutoUpdate:
        // For Download -AND- for Reloader.exe
        //
		wchar_t    path[MAX_PATH + 16];
        ::GetModuleFileName(NULL, path, MAX_PATH);
		wchar_t*   p = wcsrchr(path, '\\');
        if (!p)
            p = path;
        else
            p++;

        *p = 0; // erase filename
        ::SetCurrentDirectory(path);

        m_bNeedToMoveFiles = false;
        m_bNeedToRestart = false;
        m_bForceCRCCheck = bForceCRCCheck;
        m_cFilesDownloaded = 0;

        m_bSkipReloader = bSkipReloader;

        assert(pSink);
        m_pSink = pSink;

        m_phase = PHASE_GETTING_FILELIST;
        m_pSink->OnBeginRetrievingFileList();

        ::CreateDirectory(L"AutoUpdate", NULL);

        //
        // See if the correct file list happens to be in the AutoUpdate folder
        //
		wchar_t szBuffer[100 + MAX_PATH];
        int nLocalCRC = FileCRC(L".\\AutoUpdate\\FileList.txt", szBuffer);
        if(nLocalCRC == m_nOfficialCRC)
        {
            _debugf(L"Skipping download of FileList.txt since we already have the correct one in the AutoUpdate folder\n");

            m_pSink->OnRetrievingFileListProgress(m_nFileListSize, m_nFileListSize);

            BeginLocalFilesAnalysis();

            if (m_bErrorHasOccurred)
                delete this;
            return;
        }

        //
        // We need to download the File List
        //
        ConnectToHTTPSite();

		wchar_t szURL[256];
        Strcpy(szURL, m_szURL);
        if (m_szFilelistSubDir[0] != '\0')
        {
           Strcat(szURL, m_szFilelistSubDir);
        }
        Strcat(szURL, L"FileList.txt");
		wchar_t * szInitialList[] = { szURL, L"FileList.txt", NULL };

        m_pHTTPSession->InitiateDownload(szInitialList, L".\\AutoUpdate\\");
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    /****************************************************************************
     * Remarks:
     *    On slow machines the graphics engine hogs CPU time when updating the
     * screen.  But we don't want to completely starve it.  We want the user
     * to be able to click on the "Abort" button if they want.  So we give
     * the user about 2 frames per second for moving the mouse.
     *
     */

    void HandleAutoDownload(DWORD dwTimeAlloted)
    {
        DWORD timeStart = timeGetTime();

        do
        {
            if (m_bErrorHasOccurred)
            {
                delete this;
                return;
            }

            switch(m_phase)
            {
                case PHASE_VERIFYING_LOCAL:

                    assert(m_pLocalFilesVerifier);
                    if(!ContinueLocalFilesVerification(m_pLocalFilesVerifier, dwTimeAlloted))
                    {
                        DestroyFilesVerifier(m_pLocalFilesVerifier);
                        m_pLocalFilesVerifier = NULL;

                        OnFinishLocalFilesAnalysis();
                        return;
                    }
                    break;

                case PHASE_MAIN_DOWNLOAD:
                case PHASE_GETTING_FILELIST:

                    //
                    // If downloading,
                    // download another block
                    //
                    if(m_pHTTPSession && !m_pHTTPSession->ContinueDownload())
                    {
                        //
                        // At this point we are done downloading either FileList.txt or the actual game files
                        //
                        if (m_bErrorHasOccurred)
                        {
                            delete this;
                            return;
                        }

                        if (IsDownloadListBuilt())
                        {
                            MoveFilesThenRestartOrRelogOn();
                            return;
                        }

                        //
                        //  Now that we are done downloading FileList.txt, scan it and determine what other
                        //  files need to be downloaded.
                        //
                        BeginLocalFilesAnalysis();
                    }
                    break;
            }

        } while (timeGetTime() < timeStart + dwTimeAlloted);  // spend some time doing this
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    void Abort()
    {
        if (m_pHTTPSession)
            m_pHTTPSession->Abort();

        // treat like error
        DoError(L"Aborted.");

        m_pSink->OnUserAbort();
    }




public:

    ///////////////////////////////////////////////////////////////////////////////////////
    //
    // Methods used by CLocalFilesVerifier
    //
    ///////////////////////////////////////////////////////////////////////////////////////

    void DoError(wchar_t * szFormat, ...)
    {
		wchar_t szMsg[sizeof(m_szErrorMessage)];
        va_list pArg;
        va_start(pArg, szFormat);
		_vsnwprintf(szMsg, sizeof(szMsg), szFormat, pArg);
        va_end(pArg);

        DoError(0, szMsg);
    }

	void DoError(int nInternalErrorCode, wchar_t * szFormat, ...)
    {
        if(!m_bErrorHasOccurred)  // first error is most important
        {
            int nLastError = GetLastError();

			wchar_t szMsg[sizeof(m_szErrorMessage)];
            va_list pArg;
            va_start(pArg, szFormat);
            _vsnwprintf(szMsg, sizeof(szMsg), szFormat, pArg);
            va_end(pArg);

            ZDebugOutput(szMsg);

            // write to file
			wchar_t szFileData[sizeof(szMsg) + 200];
            SYSTEMTIME st;
            GetLocalTime(&st);
            swprintf(szFileData, L"\r\n\r\nUpdate attempt at: %d/%d/%02d at %d:%02d:%02d  (local time)\r\n%s\r\n\r\n", st.wMonth, st.wDay, st.wYear % 100, st.wHour, st.wMinute, st.wSecond, szMsg);
            bool bErrorFileSaved = UTL::AppendFile(L"UpdateError.txt", szFileData, wcslen(szFileData));

			wchar_t szDisplayMsg[256] = { L"?" };

            //
            // Suggest a resolution
            //
            if (_wcsicmp(szFormat, L"Aborted.") == 0)
            {
                swprintf(szDisplayMsg, L"Aborted.");
            }
            else
            {
                if (nInternalErrorCode == 1)
                {
                    swprintf(szDisplayMsg, L"Server is too busy.  Try again later.");
                }
                else
                if (nLastError == ERROR_DISK_FULL)
                {
                    swprintf(szDisplayMsg, L"Disk didn't have enough space.  Free up some space and try again.");
                }
                else
                if (nLastError == ERROR_INTERNET_DISCONNECTED)
                {
                    swprintf(szDisplayMsg, L"Disconnected.  Reconnect and try again.");
                }
                else
                {
                    swprintf(szDisplayMsg, L"Try again later.  As a last resort you may want to reinstall.");
                }

                if(bErrorFileSaved)
                {
                   Strcat(szDisplayMsg, L"\n\nSee UpdateError.txt for details.");
                }
                else
                {
                   Strcat(szDisplayMsg, L"\n\nUpdateError.txt could not be updated.");
                }
            }

            m_bErrorHasOccurred = true;

            Strncpy(m_szErrorMessage, szMsg, sizeof(szMsg)-1);

            m_pSink->OnError(szDisplayMsg); // forward event
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    void AddFileToDownloadList(wchar_t * szFileName, unsigned long cFileSize, int nCRC, SYSTEMTIME * pstTime)
    {
        _debugf(L"Planning to download %s.\n", szFileName);

        if (_wcsicmp(szFileName, L"FileList.txt") == 0)
        {
            DoError(L"FileList.txt is invalid. The file \"FileList.txt\" was found within FileList.txt. ");
            return;
        }

        //
        // Expand file list memory as needed (or initialize list memory)
        //
        if ((m_cFiles + 1) >= m_cListAllocSize)
        {
            m_cListAllocSize = max(32, m_cListAllocSize*2);

			m_pszFileList = (wchar_t**)realloc(m_pszFileList, m_cListAllocSize * sizeof(wchar_t*));

            m_pFileInfo = (CFileInfo*)realloc(m_pFileInfo, m_cListAllocSize * sizeof(CFileInfo));
         }

        //
        //  Add to list
        //
        m_pszFileList[m_cFiles] = _wcsdup(szFileName); // Alloc space for filename and copy it

        m_pFileInfo[m_cFiles].nCRC = nCRC;
        m_pFileInfo[m_cFiles].nSize = cFileSize;
        m_pFileInfo[m_cFiles].stTime = *pstTime;

        m_cFiles++;

        m_pszFileList[m_cFiles] = NULL; // mark end of ptr list

        m_cTotalBytes += cFileSize;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    void OnAnalysisProgress(float fProgress)
    {
        m_pSink->OnAnalysisProgress(fProgress);
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    inline bool HasErrorOccurred()
    {
        return m_bErrorHasOccurred;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    inline void SetNeedToMoveFiles(bool bNewValue)
    {
        m_bNeedToMoveFiles = bNewValue;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

	virtual bool ShouldFilterFile(const wchar_t * szFileName) // if returns true, then file is not downloaded
    {
        return m_pSink->ShouldFilterFile(szFileName);
    }

    ///////////////////////////////////////////////////////////////////////////////////////

	bool ShouldRetry(wchar_t * szFilename)
    {
        ZFile file(szFilename);

        int n = file.GetLength(); // -1 means error
        if (n != -1 && n != 0 && n < 20*1024)
        {
            char * pData = new char[n+1];
            memcpy(pData, file.GetPointer(), n);
            pData[n] = 0;

            _strupr(pData);

            if (strstr(pData, "</HTML>"))
            {
                m_cHTTPServerFails++;

                if (m_cHTTPServerFails >= 10)
                {
                    DoError(1, L"Data for file %s is wrong. Tried to download %d times.  The HTTP Update server may be too busy or missing the file. Or there may be a problem with your ISP or proxy server.  Look at this file in a text editor/web browser for details.", szFilename, m_cHTTPServerFails);
                    return false;
                }
                Sleep(500);

                return true;
            }
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

public:

    ///////////////////////////////////////////////////////////////////////////////////////
    //
    // Events associated with IFTPSessionUpdateSink
    //
    ///////////////////////////////////////////////////////////////////////////////////////

	virtual void OnError(wchar_t *szErrorMessage)
    {
        DoError(szErrorMessage);
    }

	virtual bool OnFileCompleted(wchar_t * szFileName)
    {
		wchar_t szNewFileName[MAX_PATH + 10];
        Strcpy(szNewFileName, m_szEXEPath);
        Strcat(szNewFileName, L"AutoUpdate\\");
        Strcat(szNewFileName, szFileName);

		wchar_t szOutOfSyncExplaination[] = { L"Perhaps FileList.txt in Lobby server's folder is different than one at FTP Site -OR- Newer file(s) have been added to FTP Site without updating FileList.txt." };

        int nLength = CAutoDownloadUtil::GetFileLength(szNewFileName);

        if (nLength == (unsigned) -1)
        {
            DoError(L"Couldn't determine file length for %s (Error Code %d) ", szFileName, GetLastError());
            return true;
        }

        unsigned cbFilesize = (IsDownloadListBuilt() ? m_pFileInfo[m_cFilesDownloaded].nSize : m_nFileListSize);
        int cBytesSaved = cbFilesize - nLength;

        if (cBytesSaved < 0)
        {
            if (ShouldRetry(szNewFileName))
                return false;

            DoError(L"File %s was bigger than expected.  Make sure FileList.txt is up-to-date.  Make sure no compressed files at FTP site got bigger when compressed. Make sure lobby server's FileList.txt is the same as one on FTP Site. ", szFileName);
            return true;
        }

        if (cBytesSaved > 0)
        {
            //
            // Decompress the file.
            //

			wchar_t szDestFileName[MAX_PATH + 11];
            Strcpy(szDestFileName, szNewFileName);
            Strcat(szDestFileName, L"_");

            OFSTRUCT dum1/*, dum2*/;

            unsigned cbDone = 0;

            const int cbBuffer = 128*1024;

			//NYI Imago 10/14 7z TODO
            INT nSourceHandle = LZOpenFile(szNewFileName, &dum1, OF_READ);

            if (nSourceHandle < 0)
            {
                DoError(L"Failed to open compressed file %s \n LZ Error Code: %d \n ", szNewFileName, nSourceHandle);
                return true;
            }

            HANDLE hDest = CreateFile(szDestFileName,
                                     GENERIC_WRITE,
                                     FILE_SHARE_READ,
                                     NULL,
                                     CREATE_ALWAYS,
                                     FILE_ATTRIBUTE_NORMAL,
                                     NULL);

            if (hDest == INVALID_HANDLE_VALUE)
            {
                DoError(L"Failed to create a file to decompress to. %s \n Error Code: %d \n ", szDestFileName, GetLastError());
                return true;
            }

            char * pBuffer = new char[cbBuffer];

            if (pBuffer == NULL)
            {
                DoError(L"Out of memory: bytes attempted: %d", cbBuffer);
                return true;
            }

            unsigned long cbWritten;
            //
            // This is where we actually decompress the file.
            //
            do
            {
                int nReadResult = LZRead(nSourceHandle, pBuffer, cbBuffer);

                if (nReadResult < 0) // a negative return value means error
                {
                    delete[] pBuffer;
                    LZClose(nSourceHandle);
                    ::CloseHandle(hDest);
                    DoError(L"Failed to Decompress File %s \n LZ Error Code: %d \n ", szNewFileName, nReadResult);
                    return true;
                }

                if (!WriteFile(hDest, pBuffer, nReadResult, &cbWritten, NULL))
                {
                    delete[] pBuffer;
                    LZClose(nSourceHandle);
                    ::CloseHandle(hDest);
                    DoError(L"Failed to Decompress File %s \n Write Error Code: %d \n ", szDestFileName, GetLastError());
                    return true;
                }

                assert((int)cbWritten == nReadResult);

                cbDone += cbWritten;

            } while (cbWritten != 0 && cbDone < cbFilesize);


            //
            // Post decompression
            //
            delete[] pBuffer;

            LZClose(nSourceHandle);
            if (!::CloseHandle(hDest))
            {
                DoError(L"Failed to close file after decompression; %s (Error Code:%d) \n ", szDestFileName, GetLastError());
                return true;
            }

            if (cbDone != cbFilesize)
            {
                if (ShouldRetry(szNewFileName))
                    return false;

                if (cbDone == CAutoDownloadUtil::GetFileLength(szNewFileName))
                {
                    DoError(L"File size for %s did not match size of file with same name in FileList.txt. %s", szNewFileName, szOutOfSyncExplaination);
                    return true;
                }

                DoError(L"After decompression, file %s wasn't the size expected.  %s", szDestFileName, szOutOfSyncExplaination);
                return true;
            }

            DeleteFile(szNewFileName);

            if (!MoveFile(szDestFileName, szNewFileName))
            {
                DoError(L"Rename file after decompression failed; %s to %s (Error Code:%d) \n ", szNewFileName, szDestFileName, GetLastError());
                return true;
            }
        }

        if (IsDownloadListBuilt())  // if szFileName != "FileList.txt"
        {
            // don't check CRC for FileList.txt here, it's done elsewhere -AND-
            // don't send progress update for FileList.txt, only for downloading real list of files

            //
            // Verify CRC
            //
            wchar_t szErrorMsgBuffer[100+MAX_PATH];
            int nCRC;
            nCRC = FileCRC(szNewFileName, szErrorMsgBuffer);

            if (nCRC == 0)
            {
                DoError(L"Couldn't verify downloaded file's CRC: %s\n", szErrorMsgBuffer);
                return true;
            }
            else
            if (nCRC != m_pFileInfo[m_cFilesDownloaded].nCRC)
            {
                if (ShouldRetry(szNewFileName))
                    return false;

                DoError(L"CRC for %s did not match CRC of file with same name in FileList.txt.  %s \n ", szFileName, szOutOfSyncExplaination);
                return true;
            }

            //
            // Correct filetime, to speed up next analysis
            //
            SetLocalFileTime(INVALID_HANDLE_VALUE, szNewFileName, &m_pFileInfo[m_cFilesDownloaded].stTime);

            m_cBytesCompressionSavings += cBytesSaved;

            m_cFilesDownloaded++;

            m_pSink->OnFileCompleted(szFileName); // forward event

            if (_wcsicmp(szFileName, L"readme.txt") == 0)
            {
                m_bReadmeUpdated = true;
                ShellExecute(NULL, NULL, L".\\AutoUpdate\\readme.txt", NULL, NULL, SW_SHOWNORMAL);
                Sleep(1000);  // delay so that Notepad has time to load the readme file before it is moved to the EXE Folder
            }
        }

        m_cHTTPServerFails = 0;
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

	virtual void OnProgress(unsigned long cTotalBytes, const wchar_t* szCurrentFile, unsigned long cCurrentFileBytes)
    {
        if (IsDownloadListBuilt()) // don't send progress update for FileList.txt, only for downloading real list of files
        {
            // cbUncompressed == total bytes downloaded of already completed file and current file
            // (after decompression--except for current file.
            //  We don't know if current file is compressed until we are done with it.)
            unsigned cbUncompressed = cTotalBytes + m_cBytesCompressionSavings;

            //
            // Estimate time left
            //
            unsigned cbCurrentReading = cbUncompressed;

            unsigned timeCurrent = timeGetTime();
            float ftimeElapsed = float(timeCurrent - m_timeLast);

            //
            // We don't update seconds left too often or else we can encounter precision errors.
            // Usually we update it about once a second, except at first.  In the beginning we wait
            // five seconds to get a good initial reading.
            //
            if (ftimeElapsed >= (m_cSecsLeft == -1 ? 5000:1000) && cbCurrentReading != m_cbLastReading)
            {
              assert(ftimeElapsed > 0);

              float fCurrentBytesPerMillisecond = float(cbCurrentReading - m_cbLastReading) / ftimeElapsed;

              float fWeight = 0.05f;//0.0025f;//max(min(0.50f, ftimeElapsed / 100000.0f), 0.0025f);

              if (ftimeElapsed > 5000.0f)
                fWeight = 0.25f;

              if (m_cSecsLeft == -1)
                fWeight = 1.0f;

              m_fBytesPerMillisecond = m_fBytesPerMillisecond * (1.0f-fWeight) + fCurrentBytesPerMillisecond * fWeight;

              unsigned cbLeft = m_cTotalBytes - cbUncompressed;

              unsigned cSecsLeft = int(float(cbLeft-1)/(m_fBytesPerMillisecond*1000.0f)) + 1;

              if (cbLeft == 0) // if done, then make sure estimate is right
                cSecsLeft = 0;

              m_cbLastReading = cbCurrentReading;
              m_timeLast = timeCurrent;
              m_cSecsLeft = cSecsLeft;

            }
            else m_pSink->OnProgress(cbUncompressed, szCurrentFile, cCurrentFileBytes, m_cSecsLeft); // forward event
       }
        else
        {
            assert(_wcsicmp("FileList.txt", szCurrentFile) == 0);

            m_pSink->OnRetrievingFileListProgress(m_nFileListSize, cCurrentFileBytes);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    virtual void OnTransferFinished()
    {
        if (IsDownloadListBuilt()) // don't send progress update for FileList.txt, only for downloading real list of files
        {
            m_pSink->OnTransferFinished(); // forward event
        }
        else
        {
            // if FileList is compressed than it's bar will be inaccurately small, so catch up now
            m_pSink->OnRetrievingFileListProgress(m_nFileListSize, m_nFileListSize);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////

private:

    ///////////////////////////////////////////////////////////////////////////////////////

    void ConnectToHTTPSite()
    {
        if (m_pHTTPSession == NULL)
        {
            m_pHTTPSession = CreateHTTPSession(this);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    void BeginMainDownload()
    {
        assert(IsDownloadListBuilt());

        m_phase = PHASE_MAIN_DOWNLOAD;
        m_pSink->OnBeginDownloadProgressBar(m_cTotalBytes, m_cFiles);

        ConnectToHTTPSite();

        m_fBytesPerMillisecond = 1024.0f/1000.0f; // start off with what a slow modem would do; this will quickly change if true connection rate is different
        m_cbLastReading = 0;
        m_timeLast = timeGetTime();
        m_cSecsLeft = -1;

        //
        // Convert m_pszFileList be in a format that InitiateDownload() likes,
        // that is, make string pairs like so:
        // "URL1", "localfile1", "URL2", "localfile2", ..., NULL
        //
		wchar_t ** pszURLs = new wchar_t*[2 * m_cFiles + 1];
        pszURLs[2*m_cFiles] = NULL;
		wchar_t szURL[512];
        int nBaseLen = wcslen(m_szURL);
        Strcpy(szURL, m_szURL);

        for (unsigned i = 0; i < m_cFiles; i++)
        {
            Strcpy(szURL + nBaseLen, m_pszFileList[i]);
            pszURLs[i*2] =   _wcsdup(szURL);
            pszURLs[i*2+1] = _wcsdup(m_pszFileList[i]);
        }

        m_pHTTPSession->InitiateDownload(pszURLs, L".\\AutoUpdate\\");

        for (int i = 0; i < m_cFiles * 2; i++)
            free(pszURLs[i]);

        delete[] pszURLs;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    void OnFinishLocalFilesAnalysis()
    {
        if(!m_bErrorHasOccurred)
        {
            if (IsDownloadListBuilt())
            {
                BeginMainDownload();
            }
            else
            {
                //
                // if no files need to be downloaded
                //
                if (m_bNeedToMoveFiles)
                {
                    MoveFilesThenRestartOrRelogOn();
                    return;
                }
                else
                {
                    //
                    // After analysis, we have determined that no files are
                    // needed to be downloaded
                    //
                    ::DeleteFile(L"FileList.txt");

                    // Note: considered using MoveFileEx, but win95/98 doesn't support it
                    BOOL bResult = ::MoveFile(L".\\AutoUpdate\\FileList.txt", L"FileList.txt");

                    if (!bResult)
                    {
                        DoError(L"Unable to move FileList.txt from AutoUpdate folder to current one. Error Code: %d", GetLastError());
                    }

                    delete this;
                    return;
                }
            }
        }

        if(m_bErrorHasOccurred)
        {
            delete this;
            return;
        }

        //
        // At this point we have finished building the file list.  Time to download them.
        //
    }


    ///////////////////////////////////////////////////////////////////////////////////////

    void MoveFilesThenRestartOrRelogOn()
    {
        //
        // At this point we are done downloading everything.
        //

        if (!m_bSkipReloader)
        {
            // Set registry's MoveInProgress to one, meaning move is in progress
            HKEY hKey;
            DWORD dwValue = 1;
            if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_WRITE, &hKey))
            {
              ::RegSetValueEx(hKey, L"MoveInProgress", NULL, REG_DWORD, (unsigned char*)&dwValue, sizeof(DWORD));
            }
        }

        //
        // Move Files from AutoUpdate folder to Artwork (or EXE-containing) folder
        //
		wchar_t szErrorMsg[2 * MAX_PATH + 50];

        if (!CAutoDownloadUtil::MoveFiles(L".\\AutoUpdate\\", m_szArtPath, !m_bSkipReloader, &m_bNeedToRestart, m_bSkipReloader, szErrorMsg, m_pSink))
        {
            DoError(L"Error while moving downloaded files (be sure dest file isn't already open): %s", szErrorMsg);
        }

        if (m_bNeedToRestart && !m_bSkipReloader)
        {
            // since we are going to exit process soon, we should signal restart
            m_pSink->OnAutoUpdateSystemTermination(m_bErrorHasOccurred, m_bNeedToRestart);

            if (!LaunchReloaderAndExit(m_bReadmeUpdated))
            {
				wchar_t * sz = L"Couldn't complete update process; couldn't launch Reloader.exe.";
                DoError(sz);
                ::MessageBox(NULL, sz, L"Fatal Error", MB_ICONERROR);
                ::ExitProcess(0);
            }
        }

        delete this;
    }


    ////////////////////////////////////////////////////////////////////////////

    inline bool IsDownloadListBuilt()
    {
        return !!m_cFiles;
    }

    //////////////////////////////////////////////////////////////////////////

    void BeginLocalFilesAnalysis()
    {
        assert(m_pLocalFilesVerifier == NULL);
        m_phase = PHASE_VERIFYING_LOCAL;
        m_pSink->OnBeginAnalysis();
        m_pLocalFilesVerifier = CreateLocalFilesVerifier(this, m_bForceCRCCheck, m_nOfficialCRC, m_szArtPath);
    }

    //////////////////////////////////////////////////////////////////////////



private: // Data members


	wchar_t                 m_szArtPath[MAX_PATH];
	wchar_t                 m_szEXEPath[MAX_PATH];
	wchar_t                 m_szFilelistSubDir[MAX_PATH];
    bool                 m_bNeedToMoveFiles;
    bool                 m_bNeedToRestart;
    bool                 m_bErrorHasOccurred;
    bool                 m_bForceCRCCheck;
    bool                 m_bReadmeUpdated;
    bool                 m_bSkipReloader;

    float                m_fBytesPerMillisecond;
    unsigned             m_cbLastReading;
    unsigned             m_timeLast;
    unsigned             m_cSecsLeft;

	wchar_t                 m_szURL[256];
	wchar_t                 m_szUsername[128];
	wchar_t                 m_szPassword[128];

	wchar_t **              m_pszFileList;      // list of files to download
    unsigned             m_cFiles;
    unsigned long        m_cListAllocSize;   // number of bytes for alloc-ed pointers for list
    CFileInfo *          m_pFileInfo;        // pointer to array of FileInfo's
    unsigned             m_cFilesDownloaded;
    unsigned             m_cHTTPServerFails; // number of time http server returns a web page instead of the file

    unsigned long        m_cTotalBytes;      // total bytes of files in the list (uncompressed)

    unsigned             m_cBytesCompressionSavings; // used to help keep progress bars accurate

    IHTTPSession *       m_pHTTPSession;

    int                  m_nOfficialCRC;            // CRC of lobby server's FileList.txt
    unsigned             m_nFileListSize;           // length of server's FileList.txt

	wchar_t                 m_szErrorMessage[1024];

    IAutoUpdateSink *    m_pSink;      // Event Sink for UI in wintrek/pigs

    CLocalFilesVerifier * m_pLocalFilesVerifier;

    enum PHASE
    {
        PHASE_GETTING_FILELIST,
        PHASE_VERIFYING_LOCAL,
        PHASE_MAIN_DOWNLOAD
    };

    PHASE                m_phase;

};




IAutoDownload * CreateAutoDownload()
{
    return new CAutoDownloadImpl();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------
 * CLocalFilesVerifier
 *-------------------------------------------------------------------------
 * Purpose:
 *    Scan FileList.txt and figure out which files are different that is,
 *    which files need to be downloaded.
 *
 */

class CLocalFilesVerifier
{
public:

	CLocalFilesVerifier(CAutoDownloadImpl * pCreator, bool bForceCRCCheck, int nOfficialCRC, wchar_t * szArtPath) :
        m_pCreator(pCreator),
        m_file(L".\\AutoUpdate\\FileList.txt"),
        m_szArtPath(szArtPath),
        m_bForceCRCCheck(bForceCRCCheck)
    {
        m_pStart = NULL; // set to null incase m_file.IsValid is invalid
        m_pCurrent = NULL;
        m_pEnd = NULL;
        m_nLineNumber = 0;
        //
        // Open and read FileList.txt
        //
        if(!m_file.IsValid())
        {
            m_pCreator->DoError(L"Couldn't open FileList.txt (Error Code: %d) ", GetLastError());
            return;
        }
        int nSize = m_file.GetLength();
		m_pStart = new wchar_t[nSize];
        m_pCurrent = m_pStart;
        if (m_file.Read(m_pCurrent, nSize) != (DWORD) nSize)
        {
            m_pCreator->DoError(L"Couldn't read FileList.txt (Error Code: %d) ", GetLastError());
            return;
        }
        // verify FileList.txt downloaded valid
        if (MemoryCRC(m_pStart, (unsigned)nSize) != nOfficialCRC)
        {
            m_pCreator->DoError(L"FileList.txt downloaded from FTP site is different than the one the Lobby Server has in its local directory.  Make sure these files have the same CRC. ");
            return;
        }
        m_pEnd = m_pCurrent + nSize;
    }

    ~CLocalFilesVerifier()
    {
        if (m_pStart)
        {
            delete[] m_pStart;

            if (!m_pCreator->HasErrorOccurred() && m_pCurrent != m_pEnd)
            {
                m_pCreator->DoError(L"The file fileList.txt downloaded from server has a bad format near or at line: %d. ", m_nLineNumber);
            }
        }
    }


    /*-------------------------------------------------------------------------
     * ContinueAnalysis()
     *-------------------------------------------------------------------------
     * Returns:
     *    true if analysis needs to be continued.
     *    false if analysis is done or error occurs.
     *
     */
    bool ContinueAnalysis(DWORD dwTimeAlloted)
    {
        DWORD timeStart = timeGetTime();

        do
        {
            for (int i = 0; i < 50; ++i) // check up to 50 files before even calling timeGetTime()
            {
                //
                // Parse file data
                //
                if (m_pCurrent < m_pEnd - 40)
                {
                    m_nLineNumber++;

                    SYSTEMTIME systime;

                    systime.wYear = _wtoi(m_pCurrent);

                    m_pCurrent += 4;
                    if (*(m_pCurrent++) != '/')
                        return false;

					systime.wMonth = _wtoi(m_pCurrent);

                    m_pCurrent += 2;
                    if (*(m_pCurrent++) != '/')
                        return false;

					systime.wDay = _wtoi(m_pCurrent);

                    m_pCurrent += 2;
                    if (*(m_pCurrent++) != ' ')
                        return false;

					systime.wHour = _wtoi(m_pCurrent);

                    m_pCurrent += 2;
                    if (*(m_pCurrent++) != ':')
                        return false;

					systime.wMinute = _wtoi(m_pCurrent);

                    m_pCurrent += 2;
                    if (*(m_pCurrent++) != ':')
                        return false;

					systime.wSecond = _wtoi(m_pCurrent);

                    m_pCurrent += 2;
                    if (*(m_pCurrent++) != ' ')
                        return false;

					int nFileLength = _wtoi(m_pCurrent);

                    m_pCurrent += 9;
                    if (*(m_pCurrent++) != ' ')
                        return false;

                    int nCRC = UTL::hextoi(m_pCurrent);

                    m_pCurrent += 8;
                    if (*(m_pCurrent++) != ' ')
                        return false;

                    wchar_t * pLineEnd = wcschr(m_pCurrent, '\r');
                    *pLineEnd = '\0';
					wchar_t * szm_fileName = m_pCurrent;

                    systime.wMilliseconds = 0;

                    bool bTookAlotOfCPUTime = ConsiderDownloadingFile(m_pCurrent, &systime, nCRC, nFileLength);

                    if (m_pCreator->HasErrorOccurred())
                        return false;

                    m_pCurrent = pLineEnd + 2; // + 2 to skip pass "\r\n"

                    if(bTookAlotOfCPUTime)
                        break; // had to check CRC for this file so check how long we've taken
                }
                else
                {
                    m_pCreator->OnAnalysisProgress(1.0f);

                    return false;
                }
            }

        } while (timeGetTime() < timeStart + dwTimeAlloted);  // spend some time doing this

        float fProgress = float(m_pCurrent - m_pStart) / float(m_pEnd - m_pStart);

        m_pCreator->OnAnalysisProgress(fProgress);

        return true;
    }


private:

    /*-------------------------------------------------------------------------
     * IsTimeClose()
     *-------------------------------------------------------------------------
     * Returns: true only if two system times are somewhat close, within a tolerance
     *          of cSeconds.  cSeconds must be low, less than 100 is definintely safe.
     *          If you want to go higher and really want to know the max, do the math yourself
     *
     * Remarks: AutoUpdate needs this because Win98 is flakly, and sometimes
     *          when I set the filetime, it's off by a second.
     */
    bool IsTimeClose(SYSTEMTIME * pst1, SYSTEMTIME * pst2, int cSeconds)
    {
        FILETIME ft1;
        FILETIME ft2;

        assert(cSeconds < 100 && cSeconds >= 0);

        // converting to filetime takes care of leap year and all other weird date inconsistencies
        SystemTimeToFileTime(pst1, &ft1);
        SystemTimeToFileTime(pst2, &ft2);

        __int64 t1 = *((__int64*)(&ft1));
        __int64 t2 = *((__int64*)(&ft2));

        __int64 diff = (t2 - t1);

        if ((diff >= 0 ? diff : - diff) / 10000000 <= cSeconds)
          return true;

        return false;
    }

    /*-------------------------------------------------------------------------
     * ConsiderDownloadingFile()
     *-------------------------------------------------------------------------
     * Parameters:
     *      szFileName: Raw filename without path.
     *      other params are ideal attributes of the file, attributes of the file located
     *      on the FTP server (in decompressed form)
     *
     * Purpose:
     *      Consider adding szFileName to the download list
     *
     * Returns:
     *      false: if file is was quickly considered
     *      true : if file took some CPU time to look at closely.
     */
	bool ConsiderDownloadingFile(wchar_t *szFileName, SYSTEMTIME * psystime, int nCRC, int nFileLength)
    {
        if (m_pCreator->ShouldFilterFile(szFileName))
            return false;

        SYSTEMTIME systimeLocal;
        int nLocalFileLength;

		wchar_t szFileNameWithPath[MAX_PATH + 20];
        CAutoDownloadUtil::GetFileNameWithPath(szFileNameWithPath, szFileName, m_szArtPath, L".\\");

        HANDLE hFile = OpenAndGetFileInfo(szFileNameWithPath, &systimeLocal, &nLocalFileLength);

        bool bNewFile;

        if (hFile == INVALID_HANDLE_VALUE)
        {
            if(GetLastError() != ERROR_FILE_NOT_FOUND)
            {
                m_pCreator->DoError(L"Before determining if we needed to download a file, failed open local file (%s); Error Code: %d ", szFileName, GetLastError());
                return false;
            }

            bNewFile = true;
        }
        else bNewFile = false;

        bool bIsFileLengthWrong = nFileLength != nLocalFileLength;

        if (bIsFileLengthWrong                        ||
            bNewFile                                  ||
            !IsTimeClose(psystime, &systimeLocal, 3)  || // if not within 3 seconds
            m_bForceCRCCheck)
        {
            //
            // Okay, times are different, let's be ultra-sure we need to download this file,
            // let's do a CRC check
            //
			wchar_t szBuffer[100 + MAX_PATH];
            int nLocalCRC;

            if (!bNewFile && (!bIsFileLengthWrong || m_bForceCRCCheck))
            {
                nLocalCRC = FileCRC(hFile, szBuffer);

                if (nLocalCRC == 0)
                {
                    m_pCreator->DoError(L"Error during gettings CRC: %s", szBuffer);
                    ::CloseHandle(hFile);
                    return true;
                }
            }

            swprintf(szBuffer, L".\\AutoUpdate\\%s", szFileName);

            if (bIsFileLengthWrong || nLocalCRC != nCRC || bNewFile)
            {
                //
                // Let's see if the up-to-date file happens to be in the AutoUpdate folder
                //
                nLocalCRC = FileCRC(szBuffer, NULL);
                if (nLocalCRC == nCRC)
                {
                    // no need to download this one, we've already got it!
                    // but we still need to move it
                    m_pCreator->SetNeedToMoveFiles(true);

                    _debugf(L"%s with correct CRC was found in AutoUpdate folder.  Planning to move it later.  (Resetting filetime just in case.) \n", szBuffer);
                    // make sure has correct time; even though it's probably valid
                    SetLocalFileTime(INVALID_HANDLE_VALUE, szBuffer, psystime);
                }
                else
                {
                    m_pCreator->AddFileToDownloadList(szFileName, nFileLength, nCRC, psystime);
                }
            }
            else
            {
                //
                // Okay, at this point, we already have the right file, it just has the
                // wrong time.
                //
                if (!m_bForceCRCCheck)
                    _debugf(L"Correcting filetime for %s, which was found to have wrong time but correct CRC.\n", szFileName);

                // Make sure file has right time so we don't get here again.
                SetLocalFileTime(hFile, szFileNameWithPath, psystime);

                // since we already have the right file in the EXE or Artwork folder, delete one
                // of same name in AutoUpdate folder (in the rare event that a old copy of one exists there)
                ::DeleteFile(szBuffer);
            }
            ::CloseHandle(hFile);
            return true;
        }
        ::CloseHandle(hFile);
        return false;
    }

    //////////////////////////////////////////////////////////////////////////

HANDLE OpenAndGetFileInfo(wchar_t * szFileName, SYSTEMTIME * psystimeTrue, int * pnLocalFileLength)
    {
        HANDLE hFile = CreateFile(szFileName,
                                 GENERIC_READ | GENERIC_WRITE,
                                 FILE_SHARE_READ,
                                 NULL,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            // try again with just read rights

            hFile = CreateFile(szFileName,
                               GENERIC_READ,
                               FILE_SHARE_READ,
                               NULL,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               NULL);

            if (hFile == INVALID_HANDLE_VALUE)
                return INVALID_HANDLE_VALUE;
        }

        FILETIME ft;

        BOOL b = GetFileTime(hFile, NULL, NULL, &ft);
        assert(b);

        b = FileTimeToSystemTime(&ft, psystimeTrue);
        assert(b);

        *pnLocalFileLength = ::GetFileSize(hFile, NULL);

        assert(*pnLocalFileLength != 0xFFFFFFFF);

        return hFile;
    }

private:

	wchar_t *              m_pStart;
	wchar_t *              m_pCurrent;
	wchar_t *              m_pEnd;
    ZFile               m_file;
    int                 m_nLineNumber;

    bool                m_bForceCRCCheck;
	wchar_t *              m_szArtPath;

    CAutoDownloadImpl * m_pCreator;
};


CLocalFilesVerifier * CreateLocalFilesVerifier(CAutoDownloadImpl * p, bool bForceCRCCheck, int nOfficialCRC, wchar_t * szArtPath)
{
    return new CLocalFilesVerifier(p, bForceCRCCheck, nOfficialCRC, szArtPath);
}

void DestroyFilesVerifier(CLocalFilesVerifier * p)
{
    delete p;
}

bool ContinueLocalFilesVerification(CLocalFilesVerifier * p, DWORD dwTimeAlloted)
{
    return p->ContinueAnalysis(dwTimeAlloted);
}



bool LaunchReloaderAndExit(bool bReLaunchAllegianceAsMinimized)
{
    // Need to restart allegiance

	wchar_t szCommandLine[MAX_PATH];

	wchar_t * szReadme = bReLaunchAllegianceAsMinimized ? L"-Minimized" : L"-Normal";

    // This command-line needs to be in sync with the command-line reader in Reloader.exe
    swprintf(szCommandLine, L"%ld %s %s", ::GetCurrentProcessId(), szReadme, wcschr(::GetCommandLine(), L' '));

    //
    //  Launch Reloader.exe
    //
    int nRet = (int)ShellExecute(0,
         L"Open",
         L"Reloader.exe",
         szCommandLine,  // re-feed the original command-line back into Allegiance.exe
         NULL,
         SW_SHOWNORMAL);

    if(nRet <= 32)
    {
        // check again for dev/manual builds
        int nRet2 = (int)ShellExecute(0,
             L"Open",
             L"..\\Reloader\\Reloader.exe",
             szCommandLine,  // re-feed the original command-line back into Allegiance.exe
             NULL,
             SW_SHOWNORMAL);

        if (nRet2 <= 32)
            return false;
    }

    //
    //  At this point RELOADER is running so Client needs to shutdown.
    //

    // we don't need any destructors slowing us down...time to die
    ::ExitProcess(0);

    return true;
}

//////////////////////////////////////////////////////////////////////////

void SetLocalFileTime(HANDLE hFile, wchar_t *szFileName, SYSTEMTIME * psystime)
{
    bool bOpenHere = hFile == INVALID_HANDLE_VALUE;

    if (bOpenHere)
    {
        hFile = CreateFile(szFileName,
                                 GENERIC_READ | GENERIC_WRITE,
                                 FILE_SHARE_READ,
                                 NULL,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            _debugf(L"Failed to open file before trying to reset filetime, (%s); Error Code: %d ", szFileName, GetLastError());
            return;
        }
    }

    FILETIME ft;

    BOOL b = SystemTimeToFileTime(psystime, &ft);
    assert(b);

    b = SetFileTime(hFile, NULL, NULL, &ft);

    if (!b)
        _debugf(L"Unabled to reset file time; we must only have read access.\n");

    if (bOpenHere)
        ::CloseHandle(hFile);
}


