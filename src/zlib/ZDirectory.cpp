#include "pch.h"
#include "ZDirectory.h"

#include <io.h>   // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().

ZDirectory::ZDirectory()
{
}


ZDirectory::~ZDirectory()
{
}

void ZDirectory::EnsureDirectoryExistsForFilename(char * szFilename)
{
	char * pch = strstr(szFilename, "\\");
	if (pch != NULL)
	{
		char szFilepath[MAX_PATH + 20];
		strcpy(szFilepath, szFilename);
		*strrchr(szFilepath, '\\') = '\0';
		EnsurePathExists(szFilepath);
	}
}

// http://www.blackbeltcoder.com/Articles/files/ensuring-a-path-exists
// Ensures the given path exists, creating it if needed
bool ZDirectory::EnsurePathExists(char * lpszPath)
{
	if (strcmp(".", lpszPath) == 0)
		return true;

	// Nothing to do if path already exists
	if (DirectoryExists(lpszPath))
		return true;

	// Ignore trailing backslash
	int nLen = strlen(lpszPath);
	if (lpszPath[nLen - 1] == '\\')
		nLen--;

	// Skip past drive specifier
	int nCurrLen = 0;
	if (nLen >= 3 && lpszPath[1] == ':' && lpszPath[2] == '\\')
		nCurrLen = 2;

	// We can't create root so skip past any root specifier
	while (lpszPath[nCurrLen] == '\\')
		nCurrLen++;

	// Test each component of this path, creating directories as needed
	while (nCurrLen < nLen)
	{
		// Parse next path compenent
		char * psz = strchr(lpszPath + nCurrLen, '\\');
		if (psz != NULL)
			nCurrLen = (int)(psz - lpszPath);
		else
			nCurrLen = nLen;

		// Ensure this path exists
		ZString sPath(ZString(lpszPath).Left(nCurrLen));

		bool pathExists = false;
		if (access(sPath, 0) == 0)
		{
			struct stat status;
			stat(sPath, &status);
			if (status.st_mode & S_IFDIR)
				pathExists = true;
		}

		if (!DirectoryExists((char *)(PCC)sPath))
			if (!CreateDirectoryA(sPath, NULL))
				//if (!::CreateDirectory(sPath, NULL))
				return false;

		// Skip over current backslash
		if (lpszPath[nCurrLen] != '\0')
			nCurrLen++;
	}
	return true;
}

// http://www.blackbeltcoder.com/Articles/files/ensuring-a-path-exists
// Returns true if the specified path exists and is a directory
bool ZDirectory::DirectoryExists(char * lpszPath)
{
	DWORD dw = ::GetFileAttributesA(lpszPath);
	return (dw != INVALID_FILE_ATTRIBUTES &&
		(dw & FILE_ATTRIBUTE_DIRECTORY) != 0);
}