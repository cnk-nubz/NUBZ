/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef structs_TYPES_H
#define structs_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>


namespace communication {

class InternalError;

class InvalidData;

class HelloMsg;

class MapImagesRequest;

class MapImagesResponse;

class SetMapImageRequest;


class InternalError : public ::apache::thrift::TException {
 public:

  InternalError(const InternalError&);
  InternalError& operator=(const InternalError&);
  InternalError() {
  }

  virtual ~InternalError() throw();

  bool operator == (const InternalError & /* rhs */) const
  {
    return true;
  }
  bool operator != (const InternalError &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const InternalError & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
  mutable std::string thriftTExceptionMessageHolder_;
  const char* what() const throw();
};

void swap(InternalError &a, InternalError &b);

inline std::ostream& operator<<(std::ostream& out, const InternalError& obj)
{
  obj.printTo(out);
  return out;
}


class InvalidData : public ::apache::thrift::TException {
 public:

  InvalidData(const InvalidData&);
  InvalidData& operator=(const InvalidData&);
  InvalidData() {
  }

  virtual ~InvalidData() throw();

  bool operator == (const InvalidData & /* rhs */) const
  {
    return true;
  }
  bool operator != (const InvalidData &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const InvalidData & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
  mutable std::string thriftTExceptionMessageHolder_;
  const char* what() const throw();
};

void swap(InvalidData &a, InvalidData &b);

inline std::ostream& operator<<(std::ostream& out, const InvalidData& obj)
{
  obj.printTo(out);
  return out;
}

typedef struct _HelloMsg__isset {
  _HelloMsg__isset() : num(false), msg(false) {}
  bool num :1;
  bool msg :1;
} _HelloMsg__isset;

class HelloMsg {
 public:

  HelloMsg(const HelloMsg&);
  HelloMsg& operator=(const HelloMsg&);
  HelloMsg() : num(0), msg() {
  }

  virtual ~HelloMsg() throw();
  int32_t num;
  std::string msg;

  _HelloMsg__isset __isset;

  void __set_num(const int32_t val);

  void __set_msg(const std::string& val);

  bool operator == (const HelloMsg & rhs) const
  {
    if (!(num == rhs.num))
      return false;
    if (!(msg == rhs.msg))
      return false;
    return true;
  }
  bool operator != (const HelloMsg &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const HelloMsg & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(HelloMsg &a, HelloMsg &b);

inline std::ostream& operator<<(std::ostream& out, const HelloMsg& obj)
{
  obj.printTo(out);
  return out;
}

typedef struct _MapImagesRequest__isset {
  _MapImagesRequest__isset() : acquiredLevel(false) {}
  bool acquiredLevel :1;
} _MapImagesRequest__isset;

class MapImagesRequest {
 public:

  MapImagesRequest(const MapImagesRequest&);
  MapImagesRequest& operator=(const MapImagesRequest&);
  MapImagesRequest() : acquiredLevel(0) {
  }

  virtual ~MapImagesRequest() throw();
  int32_t acquiredLevel;

  _MapImagesRequest__isset __isset;

  void __set_acquiredLevel(const int32_t val);

  bool operator == (const MapImagesRequest & rhs) const
  {
    if (__isset.acquiredLevel != rhs.__isset.acquiredLevel)
      return false;
    else if (__isset.acquiredLevel && !(acquiredLevel == rhs.acquiredLevel))
      return false;
    return true;
  }
  bool operator != (const MapImagesRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MapImagesRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MapImagesRequest &a, MapImagesRequest &b);

inline std::ostream& operator<<(std::ostream& out, const MapImagesRequest& obj)
{
  obj.printTo(out);
  return out;
}

typedef struct _MapImagesResponse__isset {
  _MapImagesResponse__isset() : version(false), levelImageUrls(false) {}
  bool version :1;
  bool levelImageUrls :1;
} _MapImagesResponse__isset;

class MapImagesResponse {
 public:

  MapImagesResponse(const MapImagesResponse&);
  MapImagesResponse& operator=(const MapImagesResponse&);
  MapImagesResponse() : version(0) {
  }

  virtual ~MapImagesResponse() throw();
  int32_t version;
  std::map<int32_t, std::string>  levelImageUrls;

  _MapImagesResponse__isset __isset;

  void __set_version(const int32_t val);

  void __set_levelImageUrls(const std::map<int32_t, std::string> & val);

  bool operator == (const MapImagesResponse & rhs) const
  {
    if (!(version == rhs.version))
      return false;
    if (!(levelImageUrls == rhs.levelImageUrls))
      return false;
    return true;
  }
  bool operator != (const MapImagesResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MapImagesResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MapImagesResponse &a, MapImagesResponse &b);

inline std::ostream& operator<<(std::ostream& out, const MapImagesResponse& obj)
{
  obj.printTo(out);
  return out;
}

typedef struct _SetMapImageRequest__isset {
  _SetMapImageRequest__isset() : level(false), filename(false) {}
  bool level :1;
  bool filename :1;
} _SetMapImageRequest__isset;

class SetMapImageRequest {
 public:

  SetMapImageRequest(const SetMapImageRequest&);
  SetMapImageRequest& operator=(const SetMapImageRequest&);
  SetMapImageRequest() : level(0), filename() {
  }

  virtual ~SetMapImageRequest() throw();
  int32_t level;
  std::string filename;

  _SetMapImageRequest__isset __isset;

  void __set_level(const int32_t val);

  void __set_filename(const std::string& val);

  bool operator == (const SetMapImageRequest & rhs) const
  {
    if (!(level == rhs.level))
      return false;
    if (!(filename == rhs.filename))
      return false;
    return true;
  }
  bool operator != (const SetMapImageRequest &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const SetMapImageRequest & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(SetMapImageRequest &a, SetMapImageRequest &b);

inline std::ostream& operator<<(std::ostream& out, const SetMapImageRequest& obj)
{
  obj.printTo(out);
  return out;
}

} // namespace

#endif
