/*-------------------------------------------------------------------------
  AutoUpdate.h
  
  Management of the AutoUpdate system
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/


class IAutoUpdate
{
public:

    virtual void     LoadSettings(HKEY hk)           = 0;
	virtual void     LoadCRC(wchar_t * szFileName) = 0;
        
	virtual wchar_t *   GetFTPServer() = 0;
	virtual wchar_t *   GetFTPInitialDir() = 0;
	virtual wchar_t *   GetFTPAccount() = 0;
	virtual wchar_t *   GetFTPPassword() = 0;

    virtual int      GetFileListCRC()    = 0;
    virtual unsigned GetFileListSize()   = 0;
};

void CreateAutoUpdate(HKEY hk, wchar_t * szFileName);

extern IAutoUpdate * g_pAutoUpdate; // this is NULL is g_pAutoUpdate is not enabled

