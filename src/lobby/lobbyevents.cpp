/*-------------------------------------------------------------------------
  LobbyEvents.cpp
  
  List of lobby event messages
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
 
#include "pch.h"

const wchar_t * g_rgszLobbyEvents[] =
{
  L"Registry String Missing/Invalid: %s; Defaulting to: %s",
  L"Registry String Missing/Invalid: %s",
  L"Registry Integer Missing/Invalid: %s; Defaulting to: %d",
  L"Allegiance Lobby Service Handler not installed",
  L"Allegiance Lobby Service stopped",
  L"Bad service request",
  L"Allegiance Lobby started",
  L"Allegiance Lobby Initialization failed (hopefully some other more descriptive event has been fired already.",
  L"Uh, where's the HKEY_CLASSES_ROOT/AppID key???",
  L"AllLobby.exe is not registered. Use \"AllLobby -RegServer|Service\" to register Lobby",
  L"Creating Lobby",
  L"Could not resolve \"%s\" specified in the GameInfoServers reg entry to a valid ip address.\n",
  L"Shutting down Lobby",
  L"Initializing Lobby",
  L"Could not host dplay session. Check for correct dplay installation",
  L"Server %s was dropped for missing roll call.",
  L"Running Lobby",
  L"ODBC Error=> %s",
  L"assertion failed: %s",
  L"Hacker alert! Invalid CD Key (%s) received from server %s for player %s!",
  L"Attempting to remove player %s from mission cookie=%x; player not found.",
  L"Corrupt player join message recieved from server %s.",
  L"Join game message for player %s from server %s for mission %x, which could not be found.",
  L"Corrupt player quit message recieved from server %s.",
  L"Server connected: %s(%s)",
  L"Server disconnected: %s.",
  L"ACK!! Servers session was lost!",
  L"Server %s paused, missions made invisible.",
  L"Server %s unpaused, missions made visible.",
  L"cTokensMax (zauth.cpp) needs to be increased",
  L"Hacker alert! Invalid zone ticket received! Don't know name (since ticket is invalid), but ip address is: %s.",
  L"IZoneAuthDecrypt::DecryptTicket failed for unpredicted reason: %x",
  L"User (ip address %s) attempted to log into secure zone lobby with no login credentials. They probably think they're connecting to the Internet Lobby.",
  L"Corrupt find player message recieved from %s(%u).",
  L"Lobby received unknown message type %d from %s(%u)",
  L"ACK!! Clients session was lost!",
  L"Created mission, cookie=%x,\n  on %s/%s (which already had %d players),\n  requested by %s",
  L"Mission gone, cookie=%x, on %s (which had %d players)",
  L"AutoUpdate System has been disabled due to missing/invalid registry items",
  L"AutoUpdate System deactivated: %s",
  L"SQL initialization failed. Check SQLConfig value in registry.",
  L"SQL Error Record:\nMessage:   %S\nProcedure: %S\nNative:    %d\nLine:      %d\nOleDB:     %S"
};



