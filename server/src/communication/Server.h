/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Server_H
#define Server_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "server_types.h"

namespace communication {

#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class ServerIf {
 public:
  virtual ~ServerIf() {}
  virtual void shutdown() = 0;
  virtual int32_t ping(const  ::communication::HelloMsg& msg) = 0;
};

class ServerIfFactory {
 public:
  typedef ServerIf Handler;

  virtual ~ServerIfFactory() {}

  virtual ServerIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(ServerIf* /* handler */) = 0;
};

class ServerIfSingletonFactory : virtual public ServerIfFactory {
 public:
  ServerIfSingletonFactory(const boost::shared_ptr<ServerIf>& iface) : iface_(iface) {}
  virtual ~ServerIfSingletonFactory() {}

  virtual ServerIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(ServerIf* /* handler */) {}

 protected:
  boost::shared_ptr<ServerIf> iface_;
};

class ServerNull : virtual public ServerIf {
 public:
  virtual ~ServerNull() {}
  void shutdown() {
    return;
  }
  int32_t ping(const  ::communication::HelloMsg& /* msg */) {
    int32_t _return = 0;
    return _return;
  }
};


class Server_shutdown_args {
 public:

  Server_shutdown_args(const Server_shutdown_args&);
  Server_shutdown_args& operator=(const Server_shutdown_args&);
  Server_shutdown_args() {
  }

  virtual ~Server_shutdown_args() throw();

  bool operator == (const Server_shutdown_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Server_shutdown_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Server_shutdown_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Server_shutdown_pargs {
 public:


  virtual ~Server_shutdown_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Server_shutdown_result {
 public:

  Server_shutdown_result(const Server_shutdown_result&);
  Server_shutdown_result& operator=(const Server_shutdown_result&);
  Server_shutdown_result() {
  }

  virtual ~Server_shutdown_result() throw();

  bool operator == (const Server_shutdown_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const Server_shutdown_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Server_shutdown_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Server_shutdown_presult {
 public:


  virtual ~Server_shutdown_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _Server_ping_args__isset {
  _Server_ping_args__isset() : msg(false) {}
  bool msg :1;
} _Server_ping_args__isset;

class Server_ping_args {
 public:

  Server_ping_args(const Server_ping_args&);
  Server_ping_args& operator=(const Server_ping_args&);
  Server_ping_args() {
  }

  virtual ~Server_ping_args() throw();
   ::communication::HelloMsg msg;

  _Server_ping_args__isset __isset;

  void __set_msg(const  ::communication::HelloMsg& val);

  bool operator == (const Server_ping_args & rhs) const
  {
    if (!(msg == rhs.msg))
      return false;
    return true;
  }
  bool operator != (const Server_ping_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Server_ping_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class Server_ping_pargs {
 public:


  virtual ~Server_ping_pargs() throw();
  const  ::communication::HelloMsg* msg;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Server_ping_result__isset {
  _Server_ping_result__isset() : success(false) {}
  bool success :1;
} _Server_ping_result__isset;

class Server_ping_result {
 public:

  Server_ping_result(const Server_ping_result&);
  Server_ping_result& operator=(const Server_ping_result&);
  Server_ping_result() : success(0) {
  }

  virtual ~Server_ping_result() throw();
  int32_t success;

  _Server_ping_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const Server_ping_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const Server_ping_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Server_ping_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _Server_ping_presult__isset {
  _Server_ping_presult__isset() : success(false) {}
  bool success :1;
} _Server_ping_presult__isset;

class Server_ping_presult {
 public:


  virtual ~Server_ping_presult() throw();
  int32_t* success;

  _Server_ping_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class ServerClient : virtual public ServerIf {
 public:
  ServerClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ServerClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void shutdown();
  void send_shutdown();
  void recv_shutdown();
  int32_t ping(const  ::communication::HelloMsg& msg);
  void send_ping(const  ::communication::HelloMsg& msg);
  int32_t recv_ping();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class ServerProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<ServerIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (ServerProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_shutdown(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  ServerProcessor(boost::shared_ptr<ServerIf> iface) :
    iface_(iface) {
    processMap_["shutdown"] = &ServerProcessor::process_shutdown;
    processMap_["ping"] = &ServerProcessor::process_ping;
  }

  virtual ~ServerProcessor() {}
};

class ServerProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  ServerProcessorFactory(const ::boost::shared_ptr< ServerIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< ServerIfFactory > handlerFactory_;
};

class ServerMultiface : virtual public ServerIf {
 public:
  ServerMultiface(std::vector<boost::shared_ptr<ServerIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~ServerMultiface() {}
 protected:
  std::vector<boost::shared_ptr<ServerIf> > ifaces_;
  ServerMultiface() {}
  void add(boost::shared_ptr<ServerIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void shutdown() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->shutdown();
    }
    ifaces_[i]->shutdown();
  }

  int32_t ping(const  ::communication::HelloMsg& msg) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ping(msg);
    }
    return ifaces_[i]->ping(msg);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class ServerConcurrentClient : virtual public ServerIf {
 public:
  ServerConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  ServerConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void shutdown();
  int32_t send_shutdown();
  void recv_shutdown(const int32_t seqid);
  int32_t ping(const  ::communication::HelloMsg& msg);
  int32_t send_ping(const  ::communication::HelloMsg& msg);
  int32_t recv_ping(const int32_t seqid);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _WIN32
  #pragma warning( pop )
#endif

} // namespace

#endif