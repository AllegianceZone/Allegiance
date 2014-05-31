/**
*	Purpose of this service is to be a single definition
*	of all operations provided to:
*	 Game clients - About half the methods in client.cpp are handled.
*	 Game servers
*	 Web servers
*	 Monitoring tools
**/


namespace cpp lobbyThriftService
namespace csharp lobbyThriftService

// Mapped from DEFINE_FEDMSG(C, LOGON_LOBBY, 265)
// application logic to handle in lobby at Lobby:Client.cpp#224 @ case FM_C_LOGON_LOBBY:
struct LoginRequest {
	1: i16		Version,
	2: i32		Time,
	3: i32		FileListCrc,
	4: string	ZoneTicket,
	5: string	AuthTicket,
	6: string	Name,	 
}

enum LoginResponseType{
	Success = 1,
	Failure = 2,
	Update	= 3,
}

struct LoginResponse {
	1: LoginResponseType Response,
	2: string Reason
}

struct Core {
	1: string FileName,
}

struct Server {
	1: string Name,
	2: string RemoteAddress,
	3: string Location,
	4: i32	  CurrentGames,
	5: i32	  MaxGames,
	6: i32	  CoreMask,
}

struct ListServersResponse {
	1: list<Core> Cores,
	2: list<Server> Servers,
}

struct JoinMissionRequest {
	1: i32 Cookie;
}

struct Guid {
	1: i32	Data1;
	2: i16  Data2;
	3: i16  Data3;
	4: string Data4;
}

struct JoinMissionResponse {
	1: bool Success,
	2: i32	Cookie,
	3: string Server,
	4: i32	Port,
	5: Guid Instance,
}

struct FindPlayerResponse {
	1: i32	Cookie,
}

struct FindPlayerRequest {
	1: string Name,
}

service LobbyService {
	LoginResponse		Login(LoginRequest request),
	ListServersResponse ListServers(),
	JoinMissionResponse JoinMission(JoinMissionRequest request),
	FindPlayerResponse	FindPlayer(FindPlayerRequest request),
}