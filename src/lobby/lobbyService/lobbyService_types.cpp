/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "lobbyService_types.h"

#include <algorithm>

namespace lobbyThriftService {

int _kLoginResponseTypeValues[] = {
  LoginResponseType::Success,
  LoginResponseType::Failure,
  LoginResponseType::Update
};
const char* _kLoginResponseTypeNames[] = {
  "Success",
  "Failure",
  "Update"
};
const std::map<int, const char*> _LoginResponseType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(3, _kLoginResponseTypeValues, _kLoginResponseTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _kCreateMissionResponseTypeValues[] = {
  CreateMissionResponseType::Success,
  CreateMissionResponseType::Failure
};
const char* _kCreateMissionResponseTypeNames[] = {
  "Success",
  "Failure"
};
const std::map<int, const char*> _CreateMissionResponseType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(2, _kCreateMissionResponseTypeValues, _kCreateMissionResponseTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

const char* LoginRequest::ascii_fingerprint = "892C9AA8E89339EED69AE6CDE95A220F";
const uint8_t LoginRequest::binary_fingerprint[16] = {0x89,0x2C,0x9A,0xA8,0xE8,0x93,0x39,0xEE,0xD6,0x9A,0xE6,0xCD,0xE9,0x5A,0x22,0x0F};

uint32_t LoginRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I16) {
          xfer += iprot->readI16(this->Version);
          this->__isset.Version = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->Time);
          this->__isset.Time = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->FileListCrc);
          this->__isset.FileListCrc = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->ZoneTicket);
          this->__isset.ZoneTicket = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->AuthTicket);
          this->__isset.AuthTicket = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Name);
          this->__isset.Name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t LoginRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("LoginRequest");

  xfer += oprot->writeFieldBegin("Version", ::apache::thrift::protocol::T_I16, 1);
  xfer += oprot->writeI16(this->Version);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Time", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->Time);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("FileListCrc", ::apache::thrift::protocol::T_I32, 3);
  xfer += oprot->writeI32(this->FileListCrc);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("ZoneTicket", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->ZoneTicket);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("AuthTicket", ::apache::thrift::protocol::T_STRING, 5);
  xfer += oprot->writeString(this->AuthTicket);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Name", ::apache::thrift::protocol::T_STRING, 6);
  xfer += oprot->writeString(this->Name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(LoginRequest &a, LoginRequest &b) {
  using ::std::swap;
  swap(a.Version, b.Version);
  swap(a.Time, b.Time);
  swap(a.FileListCrc, b.FileListCrc);
  swap(a.ZoneTicket, b.ZoneTicket);
  swap(a.AuthTicket, b.AuthTicket);
  swap(a.Name, b.Name);
  swap(a.__isset, b.__isset);
}

const char* LoginResponse::ascii_fingerprint = "19B5240589E680301A7E32DF3971EFBE";
const uint8_t LoginResponse::binary_fingerprint[16] = {0x19,0xB5,0x24,0x05,0x89,0xE6,0x80,0x30,0x1A,0x7E,0x32,0xDF,0x39,0x71,0xEF,0xBE};

uint32_t LoginResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast0;
          xfer += iprot->readI32(ecast0);
          this->Response = (LoginResponseType::type)ecast0;
          this->__isset.Response = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Reason);
          this->__isset.Reason = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t LoginResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("LoginResponse");

  xfer += oprot->writeFieldBegin("Response", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((int32_t)this->Response);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Reason", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->Reason);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(LoginResponse &a, LoginResponse &b) {
  using ::std::swap;
  swap(a.Response, b.Response);
  swap(a.Reason, b.Reason);
  swap(a.__isset, b.__isset);
}

const char* Core::ascii_fingerprint = "EFB929595D312AC8F305D5A794CFEDA1";
const uint8_t Core::binary_fingerprint[16] = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

uint32_t Core::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->FileName);
          this->__isset.FileName = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Core::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("Core");

  xfer += oprot->writeFieldBegin("FileName", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->FileName);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Core &a, Core &b) {
  using ::std::swap;
  swap(a.FileName, b.FileName);
  swap(a.__isset, b.__isset);
}

const char* Server::ascii_fingerprint = "D53ED98DD748C3A2F609DE50A8A9D6A9";
const uint8_t Server::binary_fingerprint[16] = {0xD5,0x3E,0xD9,0x8D,0xD7,0x48,0xC3,0xA2,0xF6,0x09,0xDE,0x50,0xA8,0xA9,0xD6,0xA9};

uint32_t Server::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Name);
          this->__isset.Name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->RemoteAddress);
          this->__isset.RemoteAddress = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Location);
          this->__isset.Location = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->CurrentGames);
          this->__isset.CurrentGames = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->MaxGames);
          this->__isset.MaxGames = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->CoreMask);
          this->__isset.CoreMask = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Server::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("Server");

  xfer += oprot->writeFieldBegin("Name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->Name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("RemoteAddress", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->RemoteAddress);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Location", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->Location);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("CurrentGames", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->CurrentGames);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("MaxGames", ::apache::thrift::protocol::T_I32, 5);
  xfer += oprot->writeI32(this->MaxGames);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("CoreMask", ::apache::thrift::protocol::T_I32, 6);
  xfer += oprot->writeI32(this->CoreMask);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Server &a, Server &b) {
  using ::std::swap;
  swap(a.Name, b.Name);
  swap(a.RemoteAddress, b.RemoteAddress);
  swap(a.Location, b.Location);
  swap(a.CurrentGames, b.CurrentGames);
  swap(a.MaxGames, b.MaxGames);
  swap(a.CoreMask, b.CoreMask);
  swap(a.__isset, b.__isset);
}

const char* ListServersResponse::ascii_fingerprint = "B36582441B4DBF9C597B3B3DA0CE7309";
const uint8_t ListServersResponse::binary_fingerprint[16] = {0xB3,0x65,0x82,0x44,0x1B,0x4D,0xBF,0x9C,0x59,0x7B,0x3B,0x3D,0xA0,0xCE,0x73,0x09};

uint32_t ListServersResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->Cores.clear();
            uint32_t _size1;
            ::apache::thrift::protocol::TType _etype4;
            xfer += iprot->readListBegin(_etype4, _size1);
            this->Cores.resize(_size1);
            uint32_t _i5;
            for (_i5 = 0; _i5 < _size1; ++_i5)
            {
              xfer += this->Cores[_i5].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.Cores = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->Servers.clear();
            uint32_t _size6;
            ::apache::thrift::protocol::TType _etype9;
            xfer += iprot->readListBegin(_etype9, _size6);
            this->Servers.resize(_size6);
            uint32_t _i10;
            for (_i10 = 0; _i10 < _size6; ++_i10)
            {
              xfer += this->Servers[_i10].read(iprot);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.Servers = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ListServersResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ListServersResponse");

  xfer += oprot->writeFieldBegin("Cores", ::apache::thrift::protocol::T_LIST, 1);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->Cores.size()));
    std::vector<Core> ::const_iterator _iter11;
    for (_iter11 = this->Cores.begin(); _iter11 != this->Cores.end(); ++_iter11)
    {
      xfer += (*_iter11).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Servers", ::apache::thrift::protocol::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRUCT, static_cast<uint32_t>(this->Servers.size()));
    std::vector<Server> ::const_iterator _iter12;
    for (_iter12 = this->Servers.begin(); _iter12 != this->Servers.end(); ++_iter12)
    {
      xfer += (*_iter12).write(oprot);
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ListServersResponse &a, ListServersResponse &b) {
  using ::std::swap;
  swap(a.Cores, b.Cores);
  swap(a.Servers, b.Servers);
  swap(a.__isset, b.__isset);
}

const char* JoinMissionRequest::ascii_fingerprint = "E86CACEB22240450EDCBEFC3A83970E4";
const uint8_t JoinMissionRequest::binary_fingerprint[16] = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

uint32_t JoinMissionRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->Cookie);
          this->__isset.Cookie = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t JoinMissionRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("JoinMissionRequest");

  xfer += oprot->writeFieldBegin("Cookie", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->Cookie);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(JoinMissionRequest &a, JoinMissionRequest &b) {
  using ::std::swap;
  swap(a.Cookie, b.Cookie);
  swap(a.__isset, b.__isset);
}

const char* Guid::ascii_fingerprint = "871B57F7C7A01CC327FF712FCAB65F3B";
const uint8_t Guid::binary_fingerprint[16] = {0x87,0x1B,0x57,0xF7,0xC7,0xA0,0x1C,0xC3,0x27,0xFF,0x71,0x2F,0xCA,0xB6,0x5F,0x3B};

uint32_t Guid::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->Data1);
          this->__isset.Data1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I16) {
          xfer += iprot->readI16(this->Data2);
          this->__isset.Data2 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I16) {
          xfer += iprot->readI16(this->Data3);
          this->__isset.Data3 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Data4);
          this->__isset.Data4 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Guid::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("Guid");

  xfer += oprot->writeFieldBegin("Data1", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->Data1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Data2", ::apache::thrift::protocol::T_I16, 2);
  xfer += oprot->writeI16(this->Data2);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Data3", ::apache::thrift::protocol::T_I16, 3);
  xfer += oprot->writeI16(this->Data3);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Data4", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->Data4);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Guid &a, Guid &b) {
  using ::std::swap;
  swap(a.Data1, b.Data1);
  swap(a.Data2, b.Data2);
  swap(a.Data3, b.Data3);
  swap(a.Data4, b.Data4);
  swap(a.__isset, b.__isset);
}

const char* JoinMissionResponse::ascii_fingerprint = "EFE69ED7412055E9752BEFC2F660CDC4";
const uint8_t JoinMissionResponse::binary_fingerprint[16] = {0xEF,0xE6,0x9E,0xD7,0x41,0x20,0x55,0xE9,0x75,0x2B,0xEF,0xC2,0xF6,0x60,0xCD,0xC4};

uint32_t JoinMissionResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->Success);
          this->__isset.Success = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->Cookie);
          this->__isset.Cookie = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Server);
          this->__isset.Server = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->Port);
          this->__isset.Port = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->Instance.read(iprot);
          this->__isset.Instance = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t JoinMissionResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("JoinMissionResponse");

  xfer += oprot->writeFieldBegin("Success", ::apache::thrift::protocol::T_BOOL, 1);
  xfer += oprot->writeBool(this->Success);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Cookie", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->Cookie);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Server", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->Server);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Port", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->Port);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Instance", ::apache::thrift::protocol::T_STRUCT, 5);
  xfer += this->Instance.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(JoinMissionResponse &a, JoinMissionResponse &b) {
  using ::std::swap;
  swap(a.Success, b.Success);
  swap(a.Cookie, b.Cookie);
  swap(a.Server, b.Server);
  swap(a.Port, b.Port);
  swap(a.Instance, b.Instance);
  swap(a.__isset, b.__isset);
}

const char* FindPlayerRequest::ascii_fingerprint = "EFB929595D312AC8F305D5A794CFEDA1";
const uint8_t FindPlayerRequest::binary_fingerprint[16] = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

uint32_t FindPlayerRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Name);
          this->__isset.Name = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t FindPlayerRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("FindPlayerRequest");

  xfer += oprot->writeFieldBegin("Name", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->Name);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FindPlayerRequest &a, FindPlayerRequest &b) {
  using ::std::swap;
  swap(a.Name, b.Name);
  swap(a.__isset, b.__isset);
}

const char* FindPlayerResponse::ascii_fingerprint = "E86CACEB22240450EDCBEFC3A83970E4";
const uint8_t FindPlayerResponse::binary_fingerprint[16] = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

uint32_t FindPlayerResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->Cookie);
          this->__isset.Cookie = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t FindPlayerResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("FindPlayerResponse");

  xfer += oprot->writeFieldBegin("Cookie", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->Cookie);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(FindPlayerResponse &a, FindPlayerResponse &b) {
  using ::std::swap;
  swap(a.Cookie, b.Cookie);
  swap(a.__isset, b.__isset);
}

const char* CreateMissionRequest::ascii_fingerprint = "C93D890311F28844166CF6E571EB3AC2";
const uint8_t CreateMissionRequest::binary_fingerprint[16] = {0xC9,0x3D,0x89,0x03,0x11,0xF2,0x88,0x44,0x16,0x6C,0xF6,0xE5,0x71,0xEB,0x3A,0xC2};

uint32_t CreateMissionRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Server);
          this->__isset.Server = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->Address);
          this->__isset.Address = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->IGCStaticFile);
          this->__isset.IGCStaticFile = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->GameName);
          this->__isset.GameName = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t CreateMissionRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("CreateMissionRequest");

  xfer += oprot->writeFieldBegin("Server", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->Server);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("Address", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->Address);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("IGCStaticFile", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->IGCStaticFile);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("GameName", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->GameName);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(CreateMissionRequest &a, CreateMissionRequest &b) {
  using ::std::swap;
  swap(a.Server, b.Server);
  swap(a.Address, b.Address);
  swap(a.IGCStaticFile, b.IGCStaticFile);
  swap(a.GameName, b.GameName);
  swap(a.__isset, b.__isset);
}

const char* CreateMissionResponse::ascii_fingerprint = "8BBB3D0C3B370CB38F2D1340BB79F0AA";
const uint8_t CreateMissionResponse::binary_fingerprint[16] = {0x8B,0xBB,0x3D,0x0C,0x3B,0x37,0x0C,0xB3,0x8F,0x2D,0x13,0x40,0xBB,0x79,0xF0,0xAA};

uint32_t CreateMissionResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast13;
          xfer += iprot->readI32(ecast13);
          this->Response = (CreateMissionResponseType::type)ecast13;
          this->__isset.Response = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t CreateMissionResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("CreateMissionResponse");

  xfer += oprot->writeFieldBegin("Response", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32((int32_t)this->Response);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(CreateMissionResponse &a, CreateMissionResponse &b) {
  using ::std::swap;
  swap(a.Response, b.Response);
  swap(a.__isset, b.__isset);
}

} // namespace