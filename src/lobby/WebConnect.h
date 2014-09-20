#pragma once

#using <Microsoft.WindowsAzure.Configuration.dll>
#using <Microsoft.ServiceBus.dll>
#using <Newtonsoft.Json.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;
using namespace Newtonsoft::Json;
using namespace Microsoft::ServiceBus::Messaging;
using namespace Microsoft::WindowsAzure;
using namespace Microsoft::Win32;
using namespace Newtonsoft::Json::Linq;
namespace Lobby {

	/// <summary>
	/// Summary for MyComponent
	/// </summary>
	public ref class WebConnect
	{
	public:
		WebConnect(void)
		{
			RegistryKey^ key = Microsoft::Win32::Registry::LocalMachine->OpenSubKey("Software\\Microsoft\\Microsoft Games\\Allegiance\\1.3\\AllLobby");
			// eg: "Endpoint=sb://foo.servicebus.windows.net/;SharedAccessKeyName=azlobbyfoo;SharedAccessKey=4JwtkNOdafeveaef";
			String^ conn = Convert::ToString(key->GetValue("lobbygames.azurestream.connectionstring"));
			_jbuffer = gcnew Newtonsoft::Json::Linq::JArray();
			if (!String::IsNullOrEmpty(conn))
				tc = TopicClient::CreateFromConnectionString(conn, "lobbygames");
		}
		void QueueUpdateWebsite(FMD_LS_LOBBYMISSIONINFO* missionInfo){
			if (tc){
				// convert to managed struct, then add to json object array
				JObject^ jmi = gcnew JObject(
					gcnew JProperty("cbmsg", missionInfo->cbmsg),
					gcnew JProperty("cbrgSquadIDs", missionInfo->cbrgSquadIDs),
					gcnew JProperty("cbszGameDetailsFiles", missionInfo->cbszGameDetailsFiles),
					gcnew JProperty("cbszGameName", missionInfo->cbszGameName),
					gcnew JProperty("cbszIGCStaticFile", missionInfo->cbszIGCStaticFile),
					gcnew JProperty("cbszPrivilegedUsers", missionInfo->cbszPrivilegedUsers),
					gcnew JProperty("cbszServerAddr", missionInfo->cbszServerAddr),
					gcnew JProperty("cbszServerName", missionInfo->cbszServerName),
					gcnew JProperty("cbszServerVersion", missionInfo->cbszServerVersion),
					gcnew JProperty("dwCookie", missionInfo->dwCookie),
					gcnew JProperty("dwPort", missionInfo->dwPort),
					gcnew JProperty("dwStartTime", missionInfo->dwStartTime),
					gcnew JProperty("fAllowDevelopments", missionInfo->fAllowDevelopments),
					gcnew JProperty("fAnySlotsAvailable", missionInfo->fAnySlotsAvailable),
					gcnew JProperty("fArtifacts", missionInfo->fArtifacts),
					gcnew JProperty("fConquest", missionInfo->fConquest),
					gcnew JProperty("fCountdown", missionInfo->fCountdown),
					gcnew JProperty("fCountdownStarted", missionInfo->fCountdownStarted),
					gcnew JProperty("fDeathMatch", missionInfo->fDeathMatch),
					gcnew JProperty("fEjectPods", missionInfo->fEjectPods),
					gcnew JProperty("fFlags", missionInfo->fFlags),
					gcnew JProperty("fGuaranteedSlotsAvailable", missionInfo->fGuaranteedSlotsAvailable),
					gcnew JProperty("fInProgress", missionInfo->fInProgress),
					gcnew JProperty("fInvulnerableStations", missionInfo->fInvulnerableStations),
					gcnew JProperty("fLimitedLives", missionInfo->fLimitedLives),
					gcnew JProperty("fmid", missionInfo->fmid),
					gcnew JProperty("fMSArena", missionInfo->fMSArena),
					gcnew JProperty("fProsperity", missionInfo->fProsperity),
					gcnew JProperty("fScoresCount", missionInfo->fScoresCount),
					gcnew JProperty("fSquadGame", missionInfo->fSquadGame),
					gcnew JProperty("fSquadGame", missionInfo->fSquadGame),
					gcnew JProperty("fTerritorial", missionInfo->fTerritorial),
					gcnew JProperty("ibrgSquadIDs", missionInfo->ibrgSquadIDs),
					gcnew JProperty("ibszGameDetailsFiles", missionInfo->ibszGameDetailsFiles),
					gcnew JProperty("ibszGameName", missionInfo->ibszGameName),
					gcnew JProperty("ibszIGCStaticFile", missionInfo->ibszIGCStaticFile),
					gcnew JProperty("ibszPrivilegedUsers", missionInfo->ibszPrivilegedUsers),
					gcnew JProperty("ibszServerAddr", missionInfo->ibszServerAddr),
					gcnew JProperty("ibszServerName", missionInfo->ibszServerName),
					gcnew JProperty("ibszServerVersion", missionInfo->ibszServerVersion),
					gcnew JProperty("nMaxPlayersPerGame", missionInfo->nMaxPlayersPerGame),
					gcnew JProperty("nMaxPlayersPerTeam", missionInfo->nMaxPlayersPerTeam),
					gcnew JProperty("nMaxRank", missionInfo->nMaxRank),
					gcnew JProperty("nMinPlayersPerTeam", missionInfo->nMinPlayersPerTeam),
					gcnew JProperty("nMinRank", missionInfo->nMinRank),
					gcnew JProperty("nNumNoatPlayers", missionInfo->nNumNoatPlayers),
					gcnew JProperty("nNumPlayers", missionInfo->nNumPlayers),
					gcnew JProperty("nTeams", missionInfo->nTeams)
					);
				_jbuffer->Add(jmi);
			}
		}
		void Flush(){
			if (tc){
				BrokeredMessage^ m = gcnew BrokeredMessage(_jbuffer->ToString(Formatting::None));
				tc->SendAsync(m);
			}
			_jbuffer = gcnew JArray();
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~WebConnect()
		{
		}

	private:
		Newtonsoft::Json::Linq::JArray^ _jbuffer;
		TopicClient^ tc;
	};
}
