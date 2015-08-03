//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// BT - 7/15 - CSS Integration
// CSS proxy layer for CSS SOAP service calls. 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class CCssSoap
{
private:
	char m_szCssServerDomain[256];
	char m_szCssClientServicePath[256];
	char m_szCssLobbyServicePath[256];
	char m_szCssGameDataServicePath[256];

public:
	CCssSoap(char *cssServerDomain, char *cssClientServicePath, char *cssLobbyServicePath, char *cssGameDataServicePath);
	//CCssSoap(CLobbyApp *lobbyApp);

	~CCssSoap();

	bool ValidateUserLogin(char *username, char *password, char * szReason, int & iID);
	bool GetRankForCallsign(const char* szPlayerName, int *rank, double *sigma, double *mu, int *commandRank, double *commandSigma, double *commandMu, char *rankName, int rankNameLen);
	bool GetUsernameForUsernameOrCallsign(char * szCharacterName, char * szUsername, char * szReason);
	void GetBanListSinceTimestamp(char * lastBanCheckTimestamp, char * currentTimestamp, char * lastBanList);
	bool SendPlayerScoreRecord(FMD_S_PLAYER_SCORE_UPDATE *pfmPlayerScoreUpdate);
	bool CommitPlayerScoreRecords(char * szGameGuid);
	void EncodeURL(char * url, char * token);
};