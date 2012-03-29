/**
 * @file swift_commands.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 12, 2012
 */

#ifndef SWIFT_COMMANDS_H_
#define SWIFT_COMMANDS_H_

#include "sdc.h"
#include "Swiften/Swiften.h"
#include "boost/shared_ptr.hpp"

class SwiftGoOnlineRequest : public sdc::GenericRequest<sdc::Content> {
 public:
  typedef boost::shared_ptr<SwiftGoOnlineRequest> Ref;

  void HandleRequest(sdc::Connection* connection);
  void HandleResponse();

 private:
  boost::bsignals::connection signal_connection_;
};

class SwiftContactsRequest : public sdc::GenericRequest<sdc::Items<sdc::Contact> > {
 public:
  typedef boost::shared_ptr<SwiftContactsRequest> Ref;

  void HandleRequest(sdc::Connection* connection);
  void HandleResponse(Swift::Payload::ref payload, Swift::ErrorPayload::ref error);
};

class SwiftPostsRequest : public sdc::GenericRequest<sdc::Items<sdc::Post> > {
 public:
  typedef boost::shared_ptr<SwiftPostsRequest> Ref;

  void HandleRequest(sdc::Connection* connection);
  void HandleResponse(Swift::Payload::ref payload, Swift::ErrorPayload::ref error);
};

class SwiftDeletePostRequest : public sdc::GenericRequest<sdc::Content> { // TODO: Content is not used in this request
 public:
  typedef boost::shared_ptr<SwiftDeletePostRequest> Ref;

  SwiftDeletePostRequest(const std::string &id) {
    id_ = id;
  }

  void HandleRequest(sdc::Connection* connection);
  void HandleResponse(Swift::Payload::ref payload, Swift::ErrorPayload::ref error);

 private:
  std::string id_;
};


#endif /* SWIFT_COMMANDS_H_ */
