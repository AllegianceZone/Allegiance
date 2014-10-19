//////////////////////////////////////////////////////////////////////////////
//
// Main function to include in a win32app
//
//////////////////////////////////////////////////////////////////////////////

int WINAPI Win32Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmdLine, int nCmdShow);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpszCmdLine, int nCmdShow)
{
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);

    InitAllocs();

    // OleInitialize() is needed for Ole Drag and Drop

    OleInitialize(NULL);
    //CoInitialize(NULL);

    int result = Win32Main(hInstance, hPrevInstance, lpszCmdLine, nCmdShow);

    OleUninitialize();
    //CoUninitialize();

    return result;
}

int wmain( int argc, wchar_t *argv[ ])
{
    ZString strCommandLine;

	for(int index = 1; index < argc; index++) {
        ZString str = argv[index];

        if (str.Find(' ') == -1) {
            strCommandLine += ZString(argv[index]);
        } else {
            strCommandLine += L"\"" + ZString(argv[index]) + L"\"";
        }

        if (index < argc - 1) {
            strCommandLine += L" ";
        }
    }
	return wWinMain(GetModuleHandle(NULL), NULL, (wchar_t*)(PCC)strCommandLine, SW_SHOW);
}
