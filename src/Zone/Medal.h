



class IMedalBase  // inherited by IZoneMedal
{
public:
	virtual void            SetName(const wchar_t * szName) = 0;
	virtual void            SetDescription(const wchar_t * m_szDescription) = 0;
    virtual void            SetRanking(int nRanking)                      = 0; // used for sorting
	virtual void            SetImageFileName(const wchar_t * szFileName) = 0;

	virtual wchar_t *          GetName() = 0;
	virtual wchar_t *          GetDescription() = 0;
    virtual int             GetRanking()        = 0;
};

