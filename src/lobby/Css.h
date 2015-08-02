//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// BT - 7/15 - CSS Integration
// CSS proxy layer for CSS SOAP service calls. 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

bool CssValidateUserLogin(CLobbyApp * g_pLobbyApp, CQLobbyLogonData * pqd, char * szReason, int & iID);
bool CssGetRankForCallsign(CLobbyApp * g_pLobbyApp, const char* szPlayerName, int *rank, double *sigma, double *mu, int *commandRank, double *commandSigma, double *commandMu, char *rankName, int rankNameLen);
bool CssGetUsernameForUsernameOrCallsign(CLobbyApp * g_pLobbyApp, char * szCharacterName, char * szUsername, char * szReason);
void CssGetBanListSinceTimestamp(char * lastBanCheckTimestamp, char * currentTimestamp, char * lastBanList);
bool CssSendPlayerScoreRecord(FMD_S_PLAYER_SCORE_UPDATE *pfmPlayerScoreUpdate);
bool CssCommitPlayerScoreRecords(char * szGameGuid);