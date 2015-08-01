#pragma once
class ZDirectory
{
private: 
	static bool EnsurePathExists(char * lpszPath);
	static bool DirectoryExists(char * lpszPath);

public:
	ZDirectory();
	~ZDirectory();

	static void EnsureDirectoryExistsForFilename(char * filename);
};

