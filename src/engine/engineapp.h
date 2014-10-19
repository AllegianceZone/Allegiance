#ifndef _engineapp_h_
#define _engineapp_h_

//////////////////////////////////////////////////////////////////////////////
//
// EngineApp
//
//////////////////////////////////////////////////////////////////////////////

class EngineApp : public Win32App {
private:
    TRef<Engine>           m_pengine;
    TRef<Modeler>          m_pmodeler;
    TRef<IPopupContainer>  m_ppopupContainer;
    TRef<MouseInputStream> m_pmouse;

    void ParseCommandLine(
        const ZString& strCommandLine,
        bool& bSoftwareOnly,
        bool& bPrimaryOnly,
        DWORD& dwBPP
    );

public:
    //
    // EngineApp methods
    //

    Engine*          GetEngine()         { return m_pengine;         }
    Modeler*         GetModeler()        { return m_pmodeler;        }
    IPopupContainer* GetPopupContainer() { return m_ppopupContainer; }

    void SetMouse(MouseInputStream* pmouse);

    //
    // Win32App methods
    //

    HRESULT Initialize(const ZString& strCommandLine, HWND hWindow);
    void Terminate();

    int OnException(DWORD code, ExceptionData* pdata);

    #ifdef _DEBUG
	bool OnAssert(const wchar_t* psz, const wchar_t* pszFile, int line, const wchar_t* pszModule);
    #endif

	bool IsBuildDX9(); // KGJV added - see ZLib/Win32App
};

#endif
