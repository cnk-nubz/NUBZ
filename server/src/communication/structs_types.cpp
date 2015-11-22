/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "structs_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace communication {


InternalError::~InternalError() throw() {
}


uint32_t InternalError::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
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
    xfer += iprot->skip(ftype);
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t InternalError::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("InternalError");

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(InternalError &a, InternalError &b) {
  using ::std::swap;
  (void) a;
  (void) b;
}

InternalError::InternalError(const InternalError& other0) : TException() {
  (void) other0;
}
InternalError& InternalError::operator=(const InternalError& other1) {
  (void) other1;
  return *this;
}
void InternalError::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "InternalError(";
  out << ")";
}

const char* InternalError::what() const throw() {
  try {
    std::stringstream ss;
    ss << "TException - service has thrown: " << *this;
    this->thriftTExceptionMessageHolder_ = ss.str();
    return this->thriftTExceptionMessageHolder_.c_str();
  } catch (const std::exception&) {
    return "TException - service has thrown: InternalError";
  }
}


InvalidData::~InvalidData() throw() {
}


uint32_t InvalidData::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
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
    xfer += iprot->skip(ftype);
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t InvalidData::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("InvalidData");

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(InvalidData &a, InvalidData &b) {
  using ::std::swap;
  (void) a;
  (void) b;
}

InvalidData::InvalidData(const InvalidData& other2) : TException() {
  (void) other2;
}
InvalidData& InvalidData::operator=(const InvalidData& other3) {
  (void) other3;
  return *this;
}
void InvalidData::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "InvalidData(";
  out << ")";
}

const char* InvalidData::what() const throw() {
  try {
    std::stringstream ss;
    ss << "TException - service has thrown: " << *this;
    this->thriftTExceptionMessageHolder_ = ss.str();
    return this->thriftTExceptionMessageHolder_.c_str();
  } catch (const std::exception&) {
    return "TException - service has thrown: InvalidData";
  }
}


HelloMsg::~HelloMsg() throw() {
}


void HelloMsg::__set_num(const int32_t val) {
  this->num = val;
}

void HelloMsg::__set_msg(const std::string& val) {
  this->msg = val;
}

uint32_t HelloMsg::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
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
          xfer += iprot->readI32(this->num);
          this->__isset.num = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->msg);
          this->__isset.msg = true;
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

uint32_t HelloMsg::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("HelloMsg");

  xfer += oprot->writeFieldBegin("num", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->num);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("msg", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->msg);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(HelloMsg &a, HelloMsg &b) {
  using ::std::swap;
  swap(a.num, b.num);
  swap(a.msg, b.msg);
  swap(a.__isset, b.__isset);
}

HelloMsg::HelloMsg(const HelloMsg& other4) {
  num = other4.num;
  msg = other4.msg;
  __isset = other4.__isset;
}
HelloMsg& HelloMsg::operator=(const HelloMsg& other5) {
  num = other5.num;
  msg = other5.msg;
  __isset = other5.__isset;
  return *this;
}
void HelloMsg::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "HelloMsg(";
  out << "num=" << to_string(num);
  out << ", " << "msg=" << to_string(msg);
  out << ")";
}


MapImagesRequest::~MapImagesRequest() throw() {
}


void MapImagesRequest::__set_acquiredLevel(const int32_t val) {
  this->acquiredLevel = val;
__isset.acquiredLevel = true;
}

uint32_t MapImagesRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
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
          xfer += iprot->readI32(this->acquiredLevel);
          this->__isset.acquiredLevel = true;
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

uint32_t MapImagesRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("MapImagesRequest");

  if (this->__isset.acquiredLevel) {
    xfer += oprot->writeFieldBegin("acquiredLevel", ::apache::thrift::protocol::T_I32, 1);
    xfer += oprot->writeI32(this->acquiredLevel);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(MapImagesRequest &a, MapImagesRequest &b) {
  using ::std::swap;
  swap(a.acquiredLevel, b.acquiredLevel);
  swap(a.__isset, b.__isset);
}

MapImagesRequest::MapImagesRequest(const MapImagesRequest& other6) {
  acquiredLevel = other6.acquiredLevel;
  __isset = other6.__isset;
}
MapImagesRequest& MapImagesRequest::operator=(const MapImagesRequest& other7) {
  acquiredLevel = other7.acquiredLevel;
  __isset = other7.__isset;
  return *this;
}
void MapImagesRequest::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "MapImagesRequest(";
  out << "acquiredLevel="; (__isset.acquiredLevel ? (out << to_string(acquiredLevel)) : (out << "<null>"));
  out << ")";
}


MapImagesResponse::~MapImagesResponse() throw() {
}


void MapImagesResponse::__set_version(const int32_t val) {
  this->version = val;
}

void MapImagesResponse::__set_levelImageUrls(const std::map<int32_t, std::string> & val) {
  this->levelImageUrls = val;
}

uint32_t MapImagesResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
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
          xfer += iprot->readI32(this->version);
          this->__isset.version = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_MAP) {
          {
            this->levelImageUrls.clear();
            uint32_t _size8;
            ::apache::thrift::protocol::TType _ktype9;
            ::apache::thrift::protocol::TType _vtype10;
            xfer += iprot->readMapBegin(_ktype9, _vtype10, _size8);
            uint32_t _i12;
            for (_i12 = 0; _i12 < _size8; ++_i12)
            {
              int32_t _key13;
              xfer += iprot->readI32(_key13);
              std::string& _val14 = this->levelImageUrls[_key13];
              xfer += iprot->readString(_val14);
            }
            xfer += iprot->readMapEnd();
          }
          this->__isset.levelImageUrls = true;
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

uint32_t MapImagesResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("MapImagesResponse");

  xfer += oprot->writeFieldBegin("version", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->version);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("levelImageUrls", ::apache::thrift::protocol::T_MAP, 2);
  {
    xfer += oprot->writeMapBegin(::apache::thrift::protocol::T_I32, ::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->levelImageUrls.size()));
    std::map<int32_t, std::string> ::const_iterator _iter15;
    for (_iter15 = this->levelImageUrls.begin(); _iter15 != this->levelImageUrls.end(); ++_iter15)
    {
      xfer += oprot->writeI32(_iter15->first);
      xfer += oprot->writeString(_iter15->second);
    }
    xfer += oprot->writeMapEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(MapImagesResponse &a, MapImagesResponse &b) {
  using ::std::swap;
  swap(a.version, b.version);
  swap(a.levelImageUrls, b.levelImageUrls);
  swap(a.__isset, b.__isset);
}

MapImagesResponse::MapImagesResponse(const MapImagesResponse& other16) {
  version = other16.version;
  levelImageUrls = other16.levelImageUrls;
  __isset = other16.__isset;
}
MapImagesResponse& MapImagesResponse::operator=(const MapImagesResponse& other17) {
  version = other17.version;
  levelImageUrls = other17.levelImageUrls;
  __isset = other17.__isset;
  return *this;
}
void MapImagesResponse::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "MapImagesResponse(";
  out << "version=" << to_string(version);
  out << ", " << "levelImageUrls=" << to_string(levelImageUrls);
  out << ")";
}


SetMapImageRequest::~SetMapImageRequest() throw() {
}


void SetMapImageRequest::__set_level(const int32_t val) {
  this->level = val;
}

void SetMapImageRequest::__set_filename(const std::string& val) {
  this->filename = val;
}

uint32_t SetMapImageRequest::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
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
          xfer += iprot->readI32(this->level);
          this->__isset.level = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->filename);
          this->__isset.filename = true;
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

uint32_t SetMapImageRequest::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("SetMapImageRequest");

  xfer += oprot->writeFieldBegin("level", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->level);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("filename", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->filename);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(SetMapImageRequest &a, SetMapImageRequest &b) {
  using ::std::swap;
  swap(a.level, b.level);
  swap(a.filename, b.filename);
  swap(a.__isset, b.__isset);
}

SetMapImageRequest::SetMapImageRequest(const SetMapImageRequest& other18) {
  level = other18.level;
  filename = other18.filename;
  __isset = other18.__isset;
}
SetMapImageRequest& SetMapImageRequest::operator=(const SetMapImageRequest& other19) {
  level = other19.level;
  filename = other19.filename;
  __isset = other19.__isset;
  return *this;
}
void SetMapImageRequest::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "SetMapImageRequest(";
  out << "level=" << to_string(level);
  out << ", " << "filename=" << to_string(filename);
  out << ")";
}

} // namespace
