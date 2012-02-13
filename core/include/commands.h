/**
 * @file commands.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 12, 2012
 */

#ifndef CORE_COMMANDS_H_
#define CORE_COMMANDS_H_

#include "connection.h"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"

namespace sdc {

class Request : public boost::enable_shared_from_this<Request> {
 public:
  friend class Connection;
  typedef boost::shared_ptr<Request> Ref;

  virtual void HandleRequest(Connection* connection) = 0;
  //virtual void HandleResponse(Content::Ref content) = 0;

 protected:
  Connection* connection_;

 private:
  void Send(Connection* connection) {
    connection_ = connection;
    HandleRequest(connection);
  }
};

template<class Content>
class GenericRequest : public Request {
 public:
  typedef boost::shared_ptr<Content> Ref;

  void SetCallback(boost::function<void (boost::shared_ptr<Content> ) > callback) {
    callback_ = callback;
  }

 protected:
  void HandleContent(boost::shared_ptr<Content> content) {
    assert(callback_);
    callback_(content);
    connection_->DeleteRequest(this->shared_from_this());
  }

 private:
  boost::function<void (boost::shared_ptr<Content> ) > callback_;
};

}  /* namespace sdc */

#endif /* CORE_COMMANDS_H_ */
