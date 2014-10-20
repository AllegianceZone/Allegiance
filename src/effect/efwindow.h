#ifndef _efwindow_h_
#define _efwindow_h_

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class EffectWindow : public EngineWindow {
public:
    EffectWindow(
              EffectApp* papp,
        const LPWSTR&   strCommandLine,
		const LPWSTR&   strTitle = L"",
              bool       bStartFullscreen = false,
        const WinRect&   rect             = WinRect(0, 0, -1, -1),
        const WinPoint&  sizeMin          = WinPoint(1, 1),
              HMENU      hmenu            = NULL
    );
};

#endif
