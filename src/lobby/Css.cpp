//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// BT - 7/15 - CSS Integration
// CSS proxy layer for CSS SOAP service calls. 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include <regex>

bool CssValidateUserLogin(CLobbyApp * g_pLobbyApp, CQLobbyLogonData * pqd, char * szReason, int & iID)
{
	char * cssServerDomain = g_pLobbyApp->GetCssServerDomain();
	char * cssClientServicePath = g_pLobbyApp->GetCssClientServicePath();

	debugf("cssServerDomain: %s\r\n", cssServerDomain);
	debugf("cssClientServicePath: %s\r\n", cssClientServicePath);

	char szHdrs[512];
	sprintf(szHdrs, "Content-Type: text/xml;charset=UTF-8\r\nSOAPAction: \"http://tempuri.org/IClientService/LauncherSignIn\"");

	char soapBody[4092];
	sprintf(soapBody, 
		"<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:tem=\"http://tempuri.org/\" xmlns:all=\"http://schemas.datacontract.org/2004/07/Allegiance.CommunitySecuritySystem.Server.Contracts\"> \
		   <soapenv:Header/> \
		   <soapenv:Body> \
			  <tem:LauncherSignIn> \
				 <tem:data> \
				 	<all:Password><![CDATA[%s]]></all:Password> \
				 	<all:Username><![CDATA[%s]]></all:Username> \
				 </tem:data> \
			  </tem:LauncherSignIn> \
		   </soapenv:Body> \
		</soapenv:Envelope>", pqd->szPW, pqd->szCharacterName);



	int soapLength = strlen(soapBody);

	ZString Response = UTL::DoHTTP(szHdrs, cssServerDomain, "POST", cssClientServicePath, soapBody, soapLength, true);

	char * szToken;
	char * szResponse = (char*)_alloca(10000);
	Strcpy(szResponse, (PCC)Response);

	// Parse results
	std::tr1::regex rgx("<LauncherSignInResult.*?>.*?<a:Status>(.*?)</a:Status>.*?</LauncherSignInResult>");
	std::tr1::smatch result;
	std::string strResponse(szResponse);

	if (std::tr1::regex_search(strResponse, result, rgx) == true && result.size() > 1)
	{
		char status[50];
		sprintf(status, result[1].str().c_str());

		if (stricmp(status, "Ok") == 0)
		{
			return true;
		}
		else if (stricmp(status, "InvalidCredentials") == 0)
		{
			sprintf(szReason, "Your user name or password are invalid. Go to acss.alleg.net to create an account, or reset a forgotten password.");
			return false;
		}
		else if (stricmp(status, "AccountLinked") == 0)
		{
			sprintf(szReason, "Your account has been linked to another user's account. Please log in with your main account. Please visit www.freeallegiance.org to request help in the forums.");
			return false;
		}
		else if (stricmp(status, "PermissionDenied") == 0)
		{
			sprintf(szReason, "Your account access has been denied. Please visit www.freeallegiance.org to request help in the forums.");
			return false;
		}
		else if (stricmp(status, "AccountLocked") == 0)
		{
			sprintf(szReason, "Your account has been locked. Please visit www.freeallegiance.org to request help in the forums.");
			return false;
		}
		else
		{
			sprintf(szReason, "Login failed with status: %s. Please visit www.freeallegiance.org to request help in the forums.", status);
			return false;
		}
	}

	sprintf(szReason, "A general sign in error occurred. Please visit www.freeallegiance.org to request help in the forums.");
	return false;
}


// BT - 12/21/2010 - CSS - Get all rank details from the lobby web service
bool CssGetRankForCallsign(CLobbyApp * g_pLobbyApp, const char* szPlayerName, int *rank, double *sigma, double *mu, int *commandRank, double *commandSigma, double *commandMu, char *rankName, int rankNameLen)
{
	char resultMessage[1024];
	char szURL[2064];

	char * cssServerDomain = g_pLobbyApp->GetCssServerDomain();
	char * cssClientServicePath = g_pLobbyApp->GetCssLobbyServicePath();

	char szHdrs[512];
	sprintf(szHdrs, "");
	
	// the player callsign has to be urlencoded, because it may contain '+', '?', etc.
	char callsign[128];
	char playername[128];
	Strcpy(playername, szPlayerName);
	strcpy(callsign, "");
	encodeURL(callsign, playername);

	char* baseUrl = g_pLobbyApp->RetrieveAuthAddress();
	sprintf(szURL, "%s?Action=GetRank&Callsign=%s", cssClientServicePath, callsign);

	ZString Response = UTL::DoHTTP(szHdrs, cssServerDomain, "GET", szURL, "", 0, false);

	strcpy(resultMessage, "Rank Retrieve Failed.\n\nPlease contact system admin.");

	char szResponse[10000];
	strcpy(szResponse, (PCC)Response);

	debugf("CssGetRankForCallsign(): contentLen = %ld, content = %s\r\n", Response.GetLength(), szResponse);

	int resultCode = -1;

	char localRankName[50];
	if (sscanf(szResponse, "%ld|%ld|%[^|]|%lf|%lf|%ld|%lf|%lf", &resultCode, rank, localRankName, sigma, mu, commandRank, commandSigma, commandMu) == EOF)
		resultCode = -1;

	strncpy(rankName, localRankName, rankNameLen);

	if (resultCode == 0)
		strcpy(resultMessage, "Rank retrieved.");

	debugf(resultMessage);

	return resultCode == 0;
}

bool CssGetUsernameForUsernameOrCallsign(CLobbyApp * g_pLobbyApp, char * szCharacterName, char * szUsername, char * szReason)
{
	char * cssServerDomain = g_pLobbyApp->GetCssServerDomain();
	char * cssClientServicePath = g_pLobbyApp->GetCssClientServicePath();

	debugf("cssServerDomain: %s\r\n", cssServerDomain);
	debugf("cssClientServicePath: %s\r\n", cssClientServicePath);

	char szHdrs[512];
	sprintf(szHdrs, "Content-Type: text/xml;charset=UTF-8\r\nSOAPAction: \"http://tempuri.org/IClientService/GetUsernameFromCallsignOrUsername\"");

	char soapBody[4092];
	sprintf(soapBody, 
		"<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:tem=\"http://tempuri.org/\"> \
		   <soapenv:Header/> \
		   <soapenv:Body> \
			  <tem:GetUsernameFromCallsignOrUsername> \
				 <tem:callsignOrUsername><![CDATA[%s]]></tem:callsignOrUsername> \
			  </tem:GetUsernameFromCallsignOrUsername> \
		   </soapenv:Body> \
		</soapenv:Envelope>", szCharacterName);

	int soapLength = strlen(soapBody);

	ZString Response = UTL::DoHTTP(szHdrs, cssServerDomain, "POST", cssClientServicePath, soapBody, soapLength, true);

	char * szToken;
	char * szResponse = (char*)_alloca(10000);
	Strcpy(szResponse, (PCC)Response);

	// Parse results
	std::tr1::regex rgx("<GetUsernameFromCallsignOrUsernameResult.*?>.*?<a:Username>(.*?)</a:Username>.*?</GetUsernameFromCallsignOrUsernameResult>");
	std::tr1::smatch result;
	std::string strResponse(szResponse);

	if (std::tr1::regex_search(strResponse, result, rgx) == true && result.size() > 1)
	{
		sprintf(szUsername, result[1].str().c_str());
		return true;
	}
	else
	{
		sprintf(szReason, "Your primary login couldn't be found from this callsign. Please log in with your main account. Please visit www.freeallegiance.org to request help in the forums.");
		return false;
	}

	sprintf(szReason, "A general sign in error occurred. Please visit www.freeallegiance.org to request help in the forums.");
	return false;
}

void CssGetBanListSinceTimestamp(char * lastBanCheckTimestamp, char * currentTimestamp, char * lastBanList)
{
	char * cssServerDomain = g_pLobbyApp->GetCssServerDomain();
	char * cssClientServicePath = g_pLobbyApp->GetCssClientServicePath();

	debugf("cssServerDomain: %s\r\n", cssServerDomain);
	debugf("cssClientServicePath: %s\r\n", cssClientServicePath);

	char szHdrs[512];
	sprintf(szHdrs, "Content-Type: text/xml;charset=UTF-8\r\nSOAPAction: \"http://tempuri.org/IClientService/GetBannedUsernamesAfterTimestamp\"");

	char soapBody[4092];
	sprintf(soapBody, 
		"<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:tem=\"http://tempuri.org/\"> \
		   <soapenv:Header/> \
		   <soapenv:Body> \
			  <tem:GetBannedUsernamesAfterTimestamp> \
				 <tem:unixTimeStamp>%s</tem:unixTimeStamp> \
			  </tem:GetBannedUsernamesAfterTimestamp> \
		   </soapenv:Body> \
		</soapenv:Envelope>", lastBanCheckTimestamp);

	int soapLength = strlen(soapBody);

	ZString Response = UTL::DoHTTP(szHdrs, cssServerDomain, "POST", cssClientServicePath, soapBody, soapLength, true);

	char * szToken;
	char * szResponse = (char*)_alloca(10000);
	Strcpy(szResponse, (PCC)Response);

	// Parse results
	std::tr1::regex rgxBannedUserNames("<GetBannedUsernamesAfterTimestampResult .*?>.*?<a:BannedUserNames>(.*?)</a:BannedUserNames>.*?</GetBannedUsernamesAfterTimestampResult>");
	std::tr1::regex rgxCurrentTimestamp("<GetBannedUsernamesAfterTimestampResult .*?>.*?<a:CurrentTimestamp>(.*?)</a:CurrentTimestamp>.*?</GetBannedUsernamesAfterTimestampResult>");
	std::tr1::smatch result;
	std::string strResponse(szResponse);

	lastBanList[0] = '\0';

	if (std::tr1::regex_search(strResponse, result, rgxBannedUserNames) == true && result.size() > 1)
	{
		if (strlen(result[1].str().c_str()) < 4096)
			sprintf(lastBanList, result[1].str().c_str());
	}
	
	if (std::tr1::regex_search(strResponse, result, rgxCurrentTimestamp) == true && result.size() > 1)
	{
		sprintf(currentTimestamp, result[1].str().c_str());
	}
}

bool CssSendPlayerScoreRecord(FMD_S_PLAYER_SCORE_UPDATE *pfmPlayerScoreUpdate)
{
	char * cssServerDomain = g_pLobbyApp->GetCssServerDomain();
	char * cssGameDataServicePath = g_pLobbyApp->GetCssGameDataServicePath();

	debugf("cssServerDomain: %s\r\n", cssServerDomain);
	debugf("cssGameDataServicePath: %s\r\n", cssGameDataServicePath);

	char szHdrs[512];
	sprintf(szHdrs, "Content-Type: text/xml;charset=UTF-8\r\nSOAPAction: \"http://tempuri.org/GameData/LoadPlayerData\"");

	char soapBody[4092];
	int length = 0;

	length += sprintf(soapBody,
		"<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:tem=\"http://tempuri.org/\" xmlns:all=\"http://schemas.datacontract.org/2004/07/Allegiance.CommunitySecuritySystem.Server.Contracts\"> \
		   <soapenv:Header/> \
		   <soapenv:Body> \
			  <tem:LoadPlayerData> \
				 <tem:playerData>");

	length += sprintf(soapBody + length, "<all:Artifacts>%ld</all:Artifacts>", pfmPlayerScoreUpdate->cArtifacts);
	length += sprintf(soapBody + length, "<all:Assists>%ld</all:Assists>", pfmPlayerScoreUpdate->cAssists);
	length += sprintf(soapBody + length, "<all:AsteroidsSpotted>%f</all:AsteroidsSpotted>", pfmPlayerScoreUpdate->cAsteroidsSpotted);
	length += sprintf(soapBody + length, "<all:BaseCaptures>%f</all:BaseCaptures>", pfmPlayerScoreUpdate->cBaseCaptures);
	length += sprintf(soapBody + length, "<all:BaseKills>%f</all:BaseKills>", pfmPlayerScoreUpdate->cBaseKills);
	length += sprintf(soapBody + length, "<all:BuilderKills>%f</all:BuilderKills>", pfmPlayerScoreUpdate->cBuilderKills);
	length += sprintf(soapBody + length, "<all:CarrierKills>%f</all:CarrierKills>", pfmPlayerScoreUpdate->cCarrierKills);
	length += sprintf(soapBody + length, "<all:CombatRating>%f</all:CombatRating>", pfmPlayerScoreUpdate->fCombatRating);
	length += sprintf(soapBody + length, "<all:CommandCredit>%s</all:CommandCredit>", pfmPlayerScoreUpdate->bCommandCredit ? "true" : "false");
	length += sprintf(soapBody + length, "<all:CommandLose>%s</all:CommandLose>", pfmPlayerScoreUpdate->bCommandLose ? "true" : "false");
	length += sprintf(soapBody + length, "<all:CommandWin>%s</all:CommandWin>", pfmPlayerScoreUpdate->bCommandWin ? "true" : "false");
	length += sprintf(soapBody + length, "<all:Deaths>%ld</all:Deaths>", pfmPlayerScoreUpdate->cDeaths);
	length += sprintf(soapBody + length, "<all:Ejections>%ld</all:Ejections>", pfmPlayerScoreUpdate->cEjections);
	length += sprintf(soapBody + length, "<all:Flags>%ld</all:Flags>", pfmPlayerScoreUpdate->cFlags);
	length += sprintf(soapBody + length, "<all:GameGuid>%s</all:GameGuid>", pfmPlayerScoreUpdate->szGameGuid);
	length += sprintf(soapBody + length, "<all:Kills>%ld</all:Kills>", pfmPlayerScoreUpdate->cKills);
	length += sprintf(soapBody + length, "<all:LayerKills>%f</all:LayerKills>", pfmPlayerScoreUpdate->cLayerKills);
	length += sprintf(soapBody + length, "<all:LoginUsername>%s</all:LoginUsername>", pfmPlayerScoreUpdate->szCdKey);
	length += sprintf(soapBody + length, "<all:Lose>%s</all:Lose>", pfmPlayerScoreUpdate->bLose ? "true" : "false");
	length += sprintf(soapBody + length, "<all:MinerKills>%ld</all:MinerKills>", pfmPlayerScoreUpdate->cMinerKills);
	length += sprintf(soapBody + length, "<all:PilotBaseCaptures>%ld</all:PilotBaseCaptures>", pfmPlayerScoreUpdate->cPilotBaseCaptures);
	length += sprintf(soapBody + length, "<all:PilotBaseKills>%ld</all:PilotBaseKills>", pfmPlayerScoreUpdate->cPilotBaseKills);
	length += sprintf(soapBody + length, "<all:PlayerKills>%f</all:PlayerKills>", pfmPlayerScoreUpdate->cPlayerKills);
	length += sprintf(soapBody + length, "<all:Rescues>%ld</all:Rescues>", pfmPlayerScoreUpdate->cRescues);
	length += sprintf(soapBody + length, "<all:Score>%f</all:Score>", pfmPlayerScoreUpdate->fScore);
	length += sprintf(soapBody + length, "<all:TechsRecovered>%f</all:TechsRecovered>", pfmPlayerScoreUpdate->cTechsRecovered);
	length += sprintf(soapBody + length, "<all:TimeCommanded>%f</all:TimeCommanded>", pfmPlayerScoreUpdate->dtCommanded);
	length += sprintf(soapBody + length, "<all:TimePlayed>%f</all:TimePlayed>", pfmPlayerScoreUpdate->dtPlayed);
	length += sprintf(soapBody + length, "<all:WarpsSpotted>%f</all:WarpsSpotted>", pfmPlayerScoreUpdate->cWarpsSpotted);
	length += sprintf(soapBody + length, "<all:Win>%s</all:Win>", pfmPlayerScoreUpdate->bWin ? "true" : "false");

	length += sprintf(soapBody + length, "\
				 </tem:playerData> \
			  </tem:LoadPlayerData> \
		   </soapenv:Body> \
		</soapenv:Envelope>");

	int soapLength = strlen(soapBody);

	ZString Response = UTL::DoHTTP(szHdrs, cssServerDomain, "POST", cssGameDataServicePath, soapBody, soapLength, true);

	char * szToken;
	char * szResponse = (char*)_alloca(10000);
	Strcpy(szResponse, (PCC)Response);

	return strstr(szResponse, "<a:Succeeded>true</a:Succeeded>") >= 0;
}

bool CssCommitPlayerScoreRecords(char * szGameGuid)
{
	char * cssServerDomain = g_pLobbyApp->GetCssServerDomain();
	char * cssGameDataServicePath = g_pLobbyApp->GetCssGameDataServicePath();

	debugf("cssServerDomain: %s\r\n", cssServerDomain);
	debugf("cssGameDataServicePath: %s\r\n", cssGameDataServicePath);

	char szHdrs[512];
	sprintf(szHdrs, "Content-Type: text/xml;charset=UTF-8\r\nSOAPAction: \"http://tempuri.org/GameData/CommitPlayerData\"");

	char soapBody[4092];
	int length = 0;

	length += sprintf(soapBody,
		"<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:tem=\"http://tempuri.org/\" xmlns:all=\"http://schemas.datacontract.org/2004/07/Allegiance.CommunitySecuritySystem.Server\"> \
		   <soapenv:Header/> \
		   <soapenv:Body> \
			  <tem:CommitPlayerData> \
				 <tem:request> \
					<all:GameGuid>%s</all:GameGuid> \
				 </tem:request> \
			  </tem:CommitPlayerData> \
		   </soapenv:Body> \
		</soapenv:Envelope>",
		szGameGuid);

	int soapLength = strlen(soapBody);

	ZString Response = UTL::DoHTTP(szHdrs, cssServerDomain, "POST", cssGameDataServicePath, soapBody, soapLength, true);

	char * szToken;
	char * szResponse = (char*)_alloca(10000);
	Strcpy(szResponse, (PCC)Response);

	return strstr(szResponse, "<a:Succeeded>true</a:Succeeded>") >= 0;
}