
#ifndef _DX9_ENGINE_SETTINGS_
#define _DX9_ENGINE_SETTINGS_

////////////////////////////////////////////////////////////////////////////////////////////////////
// Class is basically just a data container for storing additional engine settings
// for use in the DX9 version of the game.
// Could write accessors etc. but doesn't seem a lot of point.
////////////////////////////////////////////////////////////////////////////////////////////////////
class CDX9EngineSettings
{
public:
	CDX9EngineSettings()
	{
		mbUseTexturePackFiles = false;
		m_bVSync = false; //Spunky #265 backing out //Imago 7/10
		m_dwAA = 0;
		m_iMaxTextureSize = 0;
		m_bAutoGenMipmaps = false;
		m_bDither = false;
		m_refreshrate = 60;
		m_dwAAQual = 0;
	}
	int				m_refreshrate;
	int 			m_iMaxTextureSize; //Imago 7/18/09
	bool			mbUseTexturePackFiles;
	bool 			m_bAutoGenMipmaps; //Imago 7/18/09
	bool		    m_bDither; //Imago 7/18/09
	bool		    m_bVSync; //Imago 7/18/09
	DWORD		    m_dwAA; //Imago 7/18/09
	DWORD			m_dwAAQual; //Imago 7/18/09
};

extern CDX9EngineSettings g_DX9Settings;

#endif // _DX9_ENGINE_SETTINGS_
