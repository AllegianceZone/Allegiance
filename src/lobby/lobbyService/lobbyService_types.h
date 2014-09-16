/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef lobbyService_TYPES_H
#define lobbyService_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>


namespace lobbyThriftService {

struct LoginResponseType {
  enum type {
    Success = 1,
    Failure = 2,
    Update = 3
  };
};

extern const std::map<int, const char*> _LoginResponseType_VALUES_TO_NAMES;

struct CreateMissionResponseType {
  enum type {
    Success = 1,
    Failure = 2
  };
};

extern const std::map<int, const char*> _CreateMissionResponseType_VALUES_TO_NAMES;

typedef struct _LoginRequest__isset {
  _LoginRequest__isset() : Version(false), Time(false), FileListCrc(false), ZoneTicket(false), AuthTicket(false), Name(false) {}
  bool Version;
  bool Time;
  bool FileListCrc;
  bool ZoneTicket;
  bool AuthTicket;
  bool Name;
} _LoginRequest__isset;

class LoginRequest {
 public:

  static const char* ascii_fingerprint; // = "892C9AA8E89339EED69AE6CDE95A220F";
  static const uint8_t binary_fingerprint[16]; // = {0x89,0x2C,0x9A,0xA8,0xE8,0x93,0x39,0xEE,0xD6,0x9A,0xE6,0xCD,0xE9,0x5A,0x22,0x0F};

  LoginRequest() : Version(0), Time(0), FileListCrc(0), ZoneTicket(), AuthTicket(), Name() {
  }

  virtual ~LoginRequest() throw() {}

  int16_t Version;
  int32_t Time;
  int32_t FileListCrc;
  std::string ZoneTicket;
  std::string AuthTicket;
  std::string Name;

  _LoginRequest__isset __isset;

  void __set_Version(const int16_t val) {
    Version = val;
  }

  void __set_Time(const int32_t val) {
    Time = val;
  }

  void __set_FileListCrc(const int32_t val) {
    FileListCrc = val;
  }

  void __set_ZoneTicket(const std::string& val) {
    ZoneTicket = val;
  }

  void __set_AuthTicket(const std::string& val) {
    AuthTicket = val;
  }

  void __set_Name(const std::string& val) {
    Name = val;
  }

  bool operator == (const LoginRequest & rhs) const
  {
    if (!(Version == rhs.Version))
      return false;
    if (!(Time == rhs.Time))
      return false;
    if (!(FileListCrc == rhs.FileListCrc))
      return false;
    if (!(ZoneTicket == rhs.ZoneTicket))
      return false;
    if (!(AuthTicket == rhs.AuthTicket))
      return false;
    if (!(Name == rhs.Name))
      return false;
    return true;
  }
  bool operator != (const LoginRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LoginRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(LoginRequest &a, LoginRequest &b);

typedef struct _LoginResponse__isset {
  _LoginResponse__isset() : Response(false), Reason(false) {}
  bool Response;
  bool Reason;
} _LoginResponse__isset;

class LoginResponse {
 public:

  static const char* ascii_fingerprint; // = "19B5240589E680301A7E32DF3971EFBE";
  static const uint8_t binary_fingerprint[16]; // = {0x19,0xB5,0x24,0x05,0x89,0xE6,0x80,0x30,0x1A,0x7E,0x32,0xDF,0x39,0x71,0xEF,0xBE};

  LoginResponse() : Response((LoginResponseType::type)0), Reason() {
  }

  virtual ~LoginResponse() throw() {}

  LoginResponseType::type Response;
  std::string Reason;

  _LoginResponse__isset __isset;

  void __set_Response(const LoginResponseType::type val) {
    Response = val;
  }

  void __set_Reason(const std::string& val) {
    Reason = val;
  }

  bool operator == (const LoginResponse & rhs) const
  {
    if (!(Response == rhs.Response))
      return false;
    if (!(Reason == rhs.Reason))
      return false;
    return true;
  }
  bool operator != (const LoginResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const LoginResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(LoginResponse &a, LoginResponse &b);

typedef struct _Core__isset {
  _Core__isset() : FileName(false) {}
  bool FileName;
} _Core__isset;

class Core {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  Core() : FileName() {
  }

  virtual ~Core() throw() {}

  std::string FileName;

  _Core__isset __isset;

  void __set_FileName(const std::string& val) {
    FileName = val;
  }

  bool operator == (const Core & rhs) const
  {
    if (!(FileName == rhs.FileName))
      return false;
    return true;
  }
  bool operator != (const Core &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Core & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Core &a, Core &b);

typedef struct _Server__isset {
  _Server__isset() : Name(false), RemoteAddress(false), Location(false), CurrentGames(false), MaxGames(false), CoreMask(false) {}
  bool Name;
  bool RemoteAddress;
  bool Location;
  bool CurrentGames;
  bool MaxGames;
  bool CoreMask;
} _Server__isset;

class Server {
 public:

  static const char* ascii_fingerprint; // = "D53ED98DD748C3A2F609DE50A8A9D6A9";
  static const uint8_t binary_fingerprint[16]; // = {0xD5,0x3E,0xD9,0x8D,0xD7,0x48,0xC3,0xA2,0xF6,0x09,0xDE,0x50,0xA8,0xA9,0xD6,0xA9};

  Server() : Name(), RemoteAddress(), Location(), CurrentGames(0), MaxGames(0), CoreMask(0) {
  }

  virtual ~Server() throw() {}

  std::string Name;
  std::string RemoteAddress;
  std::string Location;
  int32_t CurrentGames;
  int32_t MaxGames;
  int32_t CoreMask;

  _Server__isset __isset;

  void __set_Name(const std::string& val) {
    Name = val;
  }

  void __set_RemoteAddress(const std::string& val) {
    RemoteAddress = val;
  }

  void __set_Location(const std::string& val) {
    Location = val;
  }

  void __set_CurrentGames(const int32_t val) {
    CurrentGames = val;
  }

  void __set_MaxGames(const int32_t val) {
    MaxGames = val;
  }

  void __set_CoreMask(const int32_t val) {
    CoreMask = val;
  }

  bool operator == (const Server & rhs) const
  {
    if (!(Name == rhs.Name))
      return false;
    if (!(RemoteAddress == rhs.RemoteAddress))
      return false;
    if (!(Location == rhs.Location))
      return false;
    if (!(CurrentGames == rhs.CurrentGames))
      return false;
    if (!(MaxGames == rhs.MaxGames))
      return false;
    if (!(CoreMask == rhs.CoreMask))
      return false;
    return true;
  }
  bool operator != (const Server &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Server & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Server &a, Server &b);

typedef struct _ListServersResponse__isset {
  _ListServersResponse__isset() : Cores(false), Servers(false) {}
  bool Cores;
  bool Servers;
} _ListServersResponse__isset;

class ListServersResponse {
 public:

  static const char* ascii_fingerprint; // = "B36582441B4DBF9C597B3B3DA0CE7309";
  static const uint8_t binary_fingerprint[16]; // = {0xB3,0x65,0x82,0x44,0x1B,0x4D,0xBF,0x9C,0x59,0x7B,0x3B,0x3D,0xA0,0xCE,0x73,0x09};

  ListServersResponse() {
  }

  virtual ~ListServersResponse() throw() {}

  std::vector<Core>  Cores;
  std::vector<Server>  Servers;

  _ListServersResponse__isset __isset;

  void __set_Cores(const std::vector<Core> & val) {
    Cores = val;
  }

  void __set_Servers(const std::vector<Server> & val) {
    Servers = val;
  }

  bool operator == (const ListServersResponse & rhs) const
  {
    if (!(Cores == rhs.Cores))
      return false;
    if (!(Servers == rhs.Servers))
      return false;
    return true;
  }
  bool operator != (const ListServersResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ListServersResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(ListServersResponse &a, ListServersResponse &b);

typedef struct _JoinMissionRequest__isset {
  _JoinMissionRequest__isset() : Cookie(false) {}
  bool Cookie;
} _JoinMissionRequest__isset;

class JoinMissionRequest {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

  JoinMissionRequest() : Cookie(0) {
  }

  virtual ~JoinMissionRequest() throw() {}

  int32_t Cookie;

  _JoinMissionRequest__isset __isset;

  void __set_Cookie(const int32_t val) {
    Cookie = val;
  }

  bool operator == (const JoinMissionRequest & rhs) const
  {
    if (!(Cookie == rhs.Cookie))
      return false;
    return true;
  }
  bool operator != (const JoinMissionRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const JoinMissionRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(JoinMissionRequest &a, JoinMissionRequest &b);

typedef struct _Guid__isset {
  _Guid__isset() : Data1(false), Data2(false), Data3(false), Data4(false) {}
  bool Data1;
  bool Data2;
  bool Data3;
  bool Data4;
} _Guid__isset;

class Guid {
 public:

  static const char* ascii_fingerprint; // = "871B57F7C7A01CC327FF712FCAB65F3B";
  static const uint8_t binary_fingerprint[16]; // = {0x87,0x1B,0x57,0xF7,0xC7,0xA0,0x1C,0xC3,0x27,0xFF,0x71,0x2F,0xCA,0xB6,0x5F,0x3B};

  Guid() : Data1(0), Data2(0), Data3(0), Data4() {
  }

  virtual ~Guid() throw() {}

  int32_t Data1;
  int16_t Data2;
  int16_t Data3;
  std::string Data4;

  _Guid__isset __isset;

  void __set_Data1(const int32_t val) {
    Data1 = val;
  }

  void __set_Data2(const int16_t val) {
    Data2 = val;
  }

  void __set_Data3(const int16_t val) {
    Data3 = val;
  }

  void __set_Data4(const std::string& val) {
    Data4 = val;
  }

  bool operator == (const Guid & rhs) const
  {
    if (!(Data1 == rhs.Data1))
      return false;
    if (!(Data2 == rhs.Data2))
      return false;
    if (!(Data3 == rhs.Data3))
      return false;
    if (!(Data4 == rhs.Data4))
      return false;
    return true;
  }
  bool operator != (const Guid &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Guid & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(Guid &a, Guid &b);

typedef struct _JoinMissionResponse__isset {
  _JoinMissionResponse__isset() : Success(false), Cookie(false), Server(false), Port(false), Instance(false) {}
  bool Success;
  bool Cookie;
  bool Server;
  bool Port;
  bool Instance;
} _JoinMissionResponse__isset;

class JoinMissionResponse {
 public:

  static const char* ascii_fingerprint; // = "EFE69ED7412055E9752BEFC2F660CDC4";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xE6,0x9E,0xD7,0x41,0x20,0x55,0xE9,0x75,0x2B,0xEF,0xC2,0xF6,0x60,0xCD,0xC4};

  JoinMissionResponse() : Success(0), Cookie(0), Server(), Port(0) {
  }

  virtual ~JoinMissionResponse() throw() {}

  bool Success;
  int32_t Cookie;
  std::string Server;
  int32_t Port;
  Guid Instance;

  _JoinMissionResponse__isset __isset;

  void __set_Success(const bool val) {
    Success = val;
  }

  void __set_Cookie(const int32_t val) {
    Cookie = val;
  }

  void __set_Server(const std::string& val) {
    Server = val;
  }

  void __set_Port(const int32_t val) {
    Port = val;
  }

  void __set_Instance(const Guid& val) {
    Instance = val;
  }

  bool operator == (const JoinMissionResponse & rhs) const
  {
    if (!(Success == rhs.Success))
      return false;
    if (!(Cookie == rhs.Cookie))
      return false;
    if (!(Server == rhs.Server))
      return false;
    if (!(Port == rhs.Port))
      return false;
    if (!(Instance == rhs.Instance))
      return false;
    return true;
  }
  bool operator != (const JoinMissionResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const JoinMissionResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(JoinMissionResponse &a, JoinMissionResponse &b);

typedef struct _FindPlayerRequest__isset {
  _FindPlayerRequest__isset() : Name(false) {}
  bool Name;
} _FindPlayerRequest__isset;

class FindPlayerRequest {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  FindPlayerRequest() : Name() {
  }

  virtual ~FindPlayerRequest() throw() {}

  std::string Name;

  _FindPlayerRequest__isset __isset;

  void __set_Name(const std::string& val) {
    Name = val;
  }

  bool operator == (const FindPlayerRequest & rhs) const
  {
    if (!(Name == rhs.Name))
      return false;
    return true;
  }
  bool operator != (const FindPlayerRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FindPlayerRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(FindPlayerRequest &a, FindPlayerRequest &b);

typedef struct _FindPlayerResponse__isset {
  _FindPlayerResponse__isset() : Cookie(false) {}
  bool Cookie;
} _FindPlayerResponse__isset;

class FindPlayerResponse {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

  FindPlayerResponse() : Cookie(0) {
  }

  virtual ~FindPlayerResponse() throw() {}

  int32_t Cookie;

  _FindPlayerResponse__isset __isset;

  void __set_Cookie(const int32_t val) {
    Cookie = val;
  }

  bool operator == (const FindPlayerResponse & rhs) const
  {
    if (!(Cookie == rhs.Cookie))
      return false;
    return true;
  }
  bool operator != (const FindPlayerResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FindPlayerResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(FindPlayerResponse &a, FindPlayerResponse &b);

typedef struct _CreateMissionRequest__isset {
  _CreateMissionRequest__isset() : Server(false), Address(false), IGCStaticFile(false), GameName(false) {}
  bool Server;
  bool Address;
  bool IGCStaticFile;
  bool GameName;
} _CreateMissionRequest__isset;

class CreateMissionRequest {
 public:

  static const char* ascii_fingerprint; // = "C93D890311F28844166CF6E571EB3AC2";
  static const uint8_t binary_fingerprint[16]; // = {0xC9,0x3D,0x89,0x03,0x11,0xF2,0x88,0x44,0x16,0x6C,0xF6,0xE5,0x71,0xEB,0x3A,0xC2};

  CreateMissionRequest() : Server(), Address(), IGCStaticFile(), GameName() {
  }

  virtual ~CreateMissionRequest() throw() {}

  std::string Server;
  std::string Address;
  std::string IGCStaticFile;
  std::string GameName;

  _CreateMissionRequest__isset __isset;

  void __set_Server(const std::string& val) {
    Server = val;
  }

  void __set_Address(const std::string& val) {
    Address = val;
  }

  void __set_IGCStaticFile(const std::string& val) {
    IGCStaticFile = val;
  }

  void __set_GameName(const std::string& val) {
    GameName = val;
  }

  bool operator == (const CreateMissionRequest & rhs) const
  {
    if (!(Server == rhs.Server))
      return false;
    if (!(Address == rhs.Address))
      return false;
    if (!(IGCStaticFile == rhs.IGCStaticFile))
      return false;
    if (!(GameName == rhs.GameName))
      return false;
    return true;
  }
  bool operator != (const CreateMissionRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CreateMissionRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(CreateMissionRequest &a, CreateMissionRequest &b);

typedef struct _CreateMissionResponse__isset {
  _CreateMissionResponse__isset() : Response(false) {}
  bool Response;
} _CreateMissionResponse__isset;

class CreateMissionResponse {
 public:

  static const char* ascii_fingerprint; // = "8BBB3D0C3B370CB38F2D1340BB79F0AA";
  static const uint8_t binary_fingerprint[16]; // = {0x8B,0xBB,0x3D,0x0C,0x3B,0x37,0x0C,0xB3,0x8F,0x2D,0x13,0x40,0xBB,0x79,0xF0,0xAA};

  CreateMissionResponse() : Response((CreateMissionResponseType::type)0) {
  }

  virtual ~CreateMissionResponse() throw() {}

  CreateMissionResponseType::type Response;

  _CreateMissionResponse__isset __isset;

  void __set_Response(const CreateMissionResponseType::type val) {
    Response = val;
  }

  bool operator == (const CreateMissionResponse & rhs) const
  {
    if (!(Response == rhs.Response))
      return false;
    return true;
  }
  bool operator != (const CreateMissionResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CreateMissionResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(CreateMissionResponse &a, CreateMissionResponse &b);

} // namespace

#endif