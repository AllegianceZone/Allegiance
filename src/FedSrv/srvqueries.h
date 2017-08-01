/*-------------------------------------------------------------------------
  srvqueries.h

  Queries using the new AllegDB stuff

  Owner:

  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _SRVQUERIES_H_
#define _SRVQUERIES_H_

#include "allegdb.h"

 // Stuff we need to logon
BEGIN_QUERY(CQLogonStats, true, "{call GetLogonStats(?, ?, ?)}")

// Stuff to remember when precessing results
DWORD dwConnectionID;                 // Remember who this is using a SAFE mechanism
DWORD dwCookie;                       // Remember what mission they want to join
bool  fValid;                         // If false we just go straight to the handler
bool  fRetry;                         // Remember whether the client should retry the logon
char  szReason[256];                  // if !fValid
char  szPassword[c_cbGamePassword];   // password entered by user
char  szCDKey[c_cbCDKey];             // Remember the CD Key that they entered.

char  szCharacterName[c_cbName];
int   characterID;
char  fCanCheat;

// CharStats
CivID   civID;
RankID  rank;
float   score;
int     minutes;
short   cBaseKills;
short   cBaseCaptures;
short   cKills;
short   rating;
short   cGamesPlayed;
short   cDeaths;
short   cWins;
short   cLosses;

// BT - 12/21/2010 - ACSS integration
double	sigma;
double	mu;
RankID	commandRank;
double	commandSigma;
double	commandMu;

BEGIN_COLUMN_MAP(CQLogonStatsData)
	COLUMN_ENTRY_TYPE(1, DBTYPE_I2, civID)
	COLUMN_ENTRY_TYPE(2, DBTYPE_I2, rank)
	COLUMN_ENTRY_TYPE(3, DBTYPE_R4, score)
	COLUMN_ENTRY_TYPE(4, DBTYPE_I4, minutes)
	COLUMN_ENTRY_TYPE(5, DBTYPE_I2, cBaseKills)
	COLUMN_ENTRY_TYPE(6, DBTYPE_I2, cBaseCaptures)
	COLUMN_ENTRY_TYPE(7, DBTYPE_I2, cKills)
	COLUMN_ENTRY_TYPE(8, DBTYPE_I2, rating)
	COLUMN_ENTRY_TYPE(9, DBTYPE_I2, cGamesPlayed)
	COLUMN_ENTRY_TYPE(10, DBTYPE_I2, cDeaths)
	COLUMN_ENTRY_TYPE(11, DBTYPE_I2, cWins)
	COLUMN_ENTRY_TYPE(12, DBTYPE_I2, cLosses)
END_COLUMN_MAP()

BEGIN_PARAM_MAP(CQLogonStatsData)
	COLUMN_ENTRY_TYPE(1, DBTYPE_STR, szCharacterName)
	COLUMN_ENTRY_TYPE(2, DBTYPE_I4, characterID)
	SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
	COLUMN_ENTRY_TYPE(3, DBTYPE_I1, fCanCheat)
END_PARAM_MAP()
END_QUERY(CQLogonStats, true)

// Player squads
BEGIN_QUERY(CQCharSquads, true, "{call GetCharSquads(?)}")

// Remember who this is using a SAFE mechanism
DWORD dwConnectionID;
DWORD dwCookie;       // Remember what mission they want to join
bool  fJoin;          // Whether we've held up the join for this info
char  szPassword[32]; // password entered by user (TODO: is this limit actually enforced anywhere?)

int   characterID;

//squad stuff
char szSquadName[31]; // constant??? Please???
int  status;
int  squadID;
int  detailedStatus;

BEGIN_COLUMN_MAP(CQCharSquadsData)
	COLUMN_ENTRY_TYPE(1, DBTYPE_STR, szSquadName)
	COLUMN_ENTRY_TYPE(2, DBTYPE_I4, status)
	COLUMN_ENTRY_TYPE(3, DBTYPE_I4, squadID)
	COLUMN_ENTRY_TYPE(4, DBTYPE_I4, detailedStatus)
END_COLUMN_MAP()

BEGIN_PARAM_MAP(CQCharSquadsData)
	COLUMN_ENTRY_TYPE(1, DBTYPE_I4, characterID)
END_PARAM_MAP()
END_QUERY(CQCharSquads, true)


// Character Stats - KGJV - set type to bool instead of char for b* params
BEGIN_QUERY(CQCharStats, false, "{Call SetCharacterStats (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}")

float Score;
bool  bScoresCount;
int   CharacterID;
short CivID;
short Rating;
float PlayerKills;
float BaseKills;
float BaseCaptures;
short Deaths;
short PilotBaseKills;
short PilotBaseCaptures;
int   Minutes;
bool  bWin;
bool  bLose;
bool  bWinCmd;
bool  bLoseCmd;
short RankOld;
float WarpsSpotted;
float AsteroidsSpotted;
float TechsRecovered;
float MinerKills;
float BuilderKills;
float LayerKills;
#if _MSC_VER >= 1310
//  char dummy[100]; // -KGJV : workaround till big bug found
#endif

BEGIN_PARAM_MAP(CQCharStatsData)
	COLUMN_ENTRY_TYPE(1, DBTYPE_I1, bScoresCount)
	COLUMN_ENTRY_TYPE(2, DBTYPE_I4, CharacterID)
	COLUMN_ENTRY_TYPE(3, DBTYPE_I2, CivID)
	COLUMN_ENTRY_TYPE(4, DBTYPE_I2, Rating)
	COLUMN_ENTRY_TYPE(5, DBTYPE_R4, WarpsSpotted)
	COLUMN_ENTRY_TYPE(6, DBTYPE_R4, AsteroidsSpotted)
	COLUMN_ENTRY_TYPE(7, DBTYPE_R4, TechsRecovered)
	COLUMN_ENTRY_TYPE(8, DBTYPE_R4, MinerKills)
	COLUMN_ENTRY_TYPE(9, DBTYPE_R4, BuilderKills)
	COLUMN_ENTRY_TYPE(10, DBTYPE_R4, LayerKills)
	COLUMN_ENTRY_TYPE(11, DBTYPE_R4, PlayerKills)
	COLUMN_ENTRY_TYPE(12, DBTYPE_R4, BaseKills)
	COLUMN_ENTRY_TYPE(13, DBTYPE_R4, BaseCaptures)
	COLUMN_ENTRY_TYPE(14, DBTYPE_I2, Deaths)
	COLUMN_ENTRY_TYPE(15, DBTYPE_I2, PilotBaseKills)
	COLUMN_ENTRY_TYPE(16, DBTYPE_I2, PilotBaseCaptures)
	COLUMN_ENTRY_TYPE(17, DBTYPE_I4, Minutes)
	COLUMN_ENTRY_TYPE(18, DBTYPE_I1, bWin)
	COLUMN_ENTRY_TYPE(19, DBTYPE_I1, bLose)
	COLUMN_ENTRY_TYPE(20, DBTYPE_I1, bWinCmd)
	COLUMN_ENTRY_TYPE(21, DBTYPE_I1, bLoseCmd)
	COLUMN_ENTRY_TYPE(22, DBTYPE_I2, RankOld)
	COLUMN_ENTRY_TYPE(23, DBTYPE_R4, Score)
END_PARAM_MAP()
END_QUERY(CQCharStats, false)


BEGIN_QUERY(CQReportSquadGame, false, "{Call SquadReportGame(?, ?, ?, ?, ?, ?)}")

int squadIDWon;
int squadIDLost1;
int squadIDLost2;
int squadIDLost3;
int squadIDLost4;
int squadIDLost5;
#if _MSC_VER >= 1310
// char dummy[100]; // -KGJV : workaround till big bug found
#endif

BEGIN_PARAM_MAP(CQReportSquadGameData)
	COLUMN_ENTRY_TYPE(1, DBTYPE_I4, squadIDWon)
	COLUMN_ENTRY_TYPE(2, DBTYPE_I4, squadIDLost1)
	COLUMN_ENTRY_TYPE(3, DBTYPE_I4, squadIDLost2)
	COLUMN_ENTRY_TYPE(4, DBTYPE_I4, squadIDLost3)
	COLUMN_ENTRY_TYPE(5, DBTYPE_I4, squadIDLost4)
	COLUMN_ENTRY_TYPE(6, DBTYPE_I4, squadIDLost5)
END_PARAM_MAP()
END_QUERY(CQReportSquadGame, false)


/////////////////////////////////////////////////////////////////////////////
// Game Results
//
BEGIN_QUERY(CQGameResults, false, "insert into GameResults (GameID,Name,WinningTeam,WinningTeamID,IsGoalConquest,IsGoalCountdown,IsGoalTeamKills,IsGoalProsperity,IsGoalArtifacts,IsGoalFlags,GoalConquest,GoalCountdown,GoalTeamKills,GoalProsperity,GoalArtifacts,GoalFlags,Duration) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")

char  szGameID[18];
char  szName[65];
char  szWinningTeam[21];
short nWinningTeamID;
bool  bIsGoalConquest;
bool  bIsGoalCountdown;
bool  bIsGoalTeamKills;
bool  bIsGoalProsperity;
bool  bIsGoalArtifacts;
bool  bIsGoalFlags;
short nGoalConquest;
long  nGoalCountdown;
short nGoalTeamKills;
float fGoalProsperity;
short nGoalArtifacts;
short nGoalFlags;
long  nDuration;
#if _MSC_VER >= 1310
//  char dummy[100]; // -KGJV : workaround till big bug found
#endif

BEGIN_PARAM_MAP(CQGameResultsData)
	COLUMN_ENTRY_TYPE(1, DBTYPE_STR, szGameID)
	COLUMN_ENTRY_TYPE(2, DBTYPE_STR, szName)
	COLUMN_ENTRY_TYPE(3, DBTYPE_STR, szWinningTeam)
	COLUMN_ENTRY_TYPE(4, DBTYPE_I2, nWinningTeamID)
	COLUMN_ENTRY_TYPE(5, DBTYPE_I1, bIsGoalConquest)
	COLUMN_ENTRY_TYPE(6, DBTYPE_I1, bIsGoalCountdown)
	COLUMN_ENTRY_TYPE(7, DBTYPE_I1, bIsGoalTeamKills)
	COLUMN_ENTRY_TYPE(8, DBTYPE_I1, bIsGoalProsperity)
	COLUMN_ENTRY_TYPE(9, DBTYPE_I1, bIsGoalArtifacts)
	COLUMN_ENTRY_TYPE(10, DBTYPE_I1, bIsGoalFlags)
	COLUMN_ENTRY_TYPE(11, DBTYPE_I2, nGoalConquest)
	COLUMN_ENTRY_TYPE(12, DBTYPE_I4, nGoalCountdown)
	COLUMN_ENTRY_TYPE(13, DBTYPE_I2, nGoalTeamKills)
	COLUMN_ENTRY_TYPE(14, DBTYPE_R4, fGoalProsperity)
	COLUMN_ENTRY_TYPE(15, DBTYPE_I2, nGoalArtifacts)
	COLUMN_ENTRY_TYPE(16, DBTYPE_I2, nGoalFlags)
	COLUMN_ENTRY_TYPE(17, DBTYPE_I4, nDuration)
END_PARAM_MAP()
END_QUERY(CQGameResults, false)


/////////////////////////////////////////////////////////////////////////////
// Team Results
//
BEGIN_QUERY(CQTeamResults, false, "insert into TeamResults(GameID,TeamID,Name,CivID,Techs,PlayerKills,BaseKills,BaseCaptures,Deaths,Ejections,Flags,Artifacts,ConquestPercent,ProsperityPercentBought,ProsperityPercentComplete,TimeEndured)values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?")

char  szGameID[18];
short nTeamID;
char  szName[c_cbName];
short nCivID;
char  szTechs[(c_ttbMax + 7) / 8 * 2]; // KGJV: warning could overflow the db / sp
short cPlayerKills;
short cBaseKills;
short cBaseCaptures;
short cDeaths;
short cEjections;
short cFlags;
short cArtifacts;
short nConquestPercent;
short nProsperityPercentBought;
short nProsperityPercentComplete;
long  nTimeEndured;
#if _MSC_VER >= 1310
// char dummy[100]; // -KGJV : workaround till big bug found
#endif

BEGIN_PARAM_MAP(CQTeamResultsData)
	COLUMN_ENTRY_TYPE(1, DBTYPE_STR, szGameID)
	COLUMN_ENTRY_TYPE(2, DBTYPE_I2, nTeamID)
	COLUMN_ENTRY_TYPE(3, DBTYPE_STR, szName)
	COLUMN_ENTRY_TYPE(4, DBTYPE_I2, nCivID)
	COLUMN_ENTRY_TYPE(5, DBTYPE_STR, szTechs)
	COLUMN_ENTRY_TYPE(6, DBTYPE_I2, cPlayerKills)
	COLUMN_ENTRY_TYPE(7, DBTYPE_I2, cBaseKills)
	COLUMN_ENTRY_TYPE(8, DBTYPE_I2, cBaseCaptures)
	COLUMN_ENTRY_TYPE(9, DBTYPE_I2, cDeaths)
	COLUMN_ENTRY_TYPE(10, DBTYPE_I2, cEjections)
	COLUMN_ENTRY_TYPE(11, DBTYPE_I2, cFlags)
	COLUMN_ENTRY_TYPE(12, DBTYPE_I2, cArtifacts)
	COLUMN_ENTRY_TYPE(13, DBTYPE_I2, nConquestPercent)
	COLUMN_ENTRY_TYPE(14, DBTYPE_I2, nProsperityPercentBought)
	COLUMN_ENTRY_TYPE(15, DBTYPE_I2, nProsperityPercentComplete)
	COLUMN_ENTRY_TYPE(16, DBTYPE_I4, nTimeEndured)
END_PARAM_MAP()
END_QUERY(CQTeamResults, false)


/////////////////////////////////////////////////////////////////////////////
// Player Results
//
BEGIN_QUERY(CQPlayerResults, false, "insert into PlayerResults(GameID,TeamID,Name,PlayerKills,BuilderKills,LayerKills,MinerKills,BaseKills,BaseCaptures,PilotBaseKills,PilotBaseCaptures,Deaths,Ejections,Rescues,Flags,Artifacts,TechsRecovered,AlephsSpotted,AsteroidsSpotted,CombatRating,Score,TimePlayed)values(?, ?, ?, ?,?, ?, ?, ?,?, ?, ?, ?,?, ?, ?, ?,?, ?, ?, ?,?, ?)")

char  szGameID[18];
short nTeamID;
char  szName[c_cbName];
short cPlayerKills;
short cBuilderKills;
short cLayerKills;
short cMinerKills;
short cBaseKills;
short cBaseCaptures;
short cPilotBaseKills;
short cPilotBaseCaptures;
short cDeaths;
short cEjections;
short cRescues;
short cFlags;
short cArtifacts;
short cTechsRecovered;
short cAlephsSpotted;
short cAsteroidsSpotted;
float fCombatRating;
float fScore;
long  nTimePlayed;
#if _MSC_VER >= 1310
char dummy[100]; // -KGJV : workaround till big bug found
#endif

BEGIN_PARAM_MAP(CQPlayerResultsData)
	COLUMN_ENTRY_TYPE(1, DBTYPE_STR, szGameID)
	COLUMN_ENTRY_TYPE(2, DBTYPE_I2, nTeamID)
	COLUMN_ENTRY_TYPE(3, DBTYPE_STR, szName)
	COLUMN_ENTRY_TYPE(4, DBTYPE_I2, cPlayerKills)
	COLUMN_ENTRY_TYPE(5, DBTYPE_I2, cBuilderKills)
	COLUMN_ENTRY_TYPE(6, DBTYPE_I2, cLayerKills)
	COLUMN_ENTRY_TYPE(7, DBTYPE_I2, cMinerKills)
	COLUMN_ENTRY_TYPE(8, DBTYPE_I2, cBaseKills)
	COLUMN_ENTRY_TYPE(9, DBTYPE_I2, cBaseCaptures)
	COLUMN_ENTRY_TYPE(10, DBTYPE_I2, cPilotBaseKills)
	COLUMN_ENTRY_TYPE(11, DBTYPE_I2, cPilotBaseCaptures)
	COLUMN_ENTRY_TYPE(12, DBTYPE_I2, cDeaths)
	COLUMN_ENTRY_TYPE(13, DBTYPE_I2, cEjections)
	COLUMN_ENTRY_TYPE(14, DBTYPE_I2, cRescues)
	COLUMN_ENTRY_TYPE(15, DBTYPE_I2, cFlags)
	COLUMN_ENTRY_TYPE(16, DBTYPE_I2, cArtifacts)
	COLUMN_ENTRY_TYPE(17, DBTYPE_I2, cTechsRecovered)
	COLUMN_ENTRY_TYPE(18, DBTYPE_I2, cAlephsSpotted)
	COLUMN_ENTRY_TYPE(19, DBTYPE_I2, cAsteroidsSpotted)
	COLUMN_ENTRY_TYPE(20, DBTYPE_R4, fCombatRating)
	COLUMN_ENTRY_TYPE(21, DBTYPE_R4, fScore)
	COLUMN_ENTRY_TYPE(22, DBTYPE_I4, nTimePlayed)
END_PARAM_MAP()
END_QUERY(CQPlayerResults, false)


/////////////////////////////////////////////////////////////////////////////
// Get Invitation List
//
BEGIN_QUERY(CQGetInvitationList, true, "select TeamIndex, rtrim(SubjectName) from InvitationLists where ListID=?")

// IN
long listid;

// OUT
char iTeam;
char szSubject[c_cbName];

// Baggage
MissionID missionID;

BEGIN_COLUMN_MAP(CQGetInvitationListData)
	COLUMN_ENTRY_TYPE(1, DBTYPE_I1, iTeam)
	COLUMN_ENTRY_TYPE(2, DBTYPE_STR, szSubject)
END_COLUMN_MAP()

BEGIN_PARAM_MAP(CQGetInvitationListData)
	COLUMN_ENTRY_TYPE(1, DBTYPE_I4, listid)
END_PARAM_MAP()

END_QUERY(CQGetInvitationList, false)


/////////////////////////////////////////////////////////////////////////////
// Event Log Record
//
BEGIN_QUERY(CQLogEvent, false, "insert into Events(Event,ComputerName,Subject,SubjectName,Context,ObjectRef)values(?, ?, ?, ?,?, ?)")

// IN
long nEvent;
char szComputerName[15 + 1];
long nSubject;
char szSubjectName[32 + 1];
char szContext[24 + 1];
char szObjectRef[4000 + 1];

// Baggage
HANDLE hevt;

BEGIN_PARAM_MAP(CQLogEventData)
	COLUMN_ENTRY_TYPE(1, DBTYPE_I4, nEvent)
	COLUMN_ENTRY_TYPE(2, DBTYPE_STR, szComputerName)
	COLUMN_ENTRY_TYPE(3, DBTYPE_I4, nSubject)
	COLUMN_ENTRY_TYPE(4, DBTYPE_STR, szSubjectName)
	COLUMN_ENTRY_TYPE(5, DBTYPE_STR, szContext)
	COLUMN_ENTRY_TYPE(6, DBTYPE_STR, szObjectRef)
END_PARAM_MAP()

END_QUERY(CQLogEvent, false)


/*-----------------------------------------------------------------------*/

#endif


