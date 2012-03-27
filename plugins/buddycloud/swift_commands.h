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

class SwiftContactsRequest : public sdc::GenericRequest<sdc::Contacts> {
 public:
  typedef boost::shared_ptr<SwiftContactsRequest> Ref;

  void HandleRequest(sdc::Connection* connection);
  void HandleResponse(Swift::Payload::ref payload, Swift::ErrorPayload::ref error);
};

class SwiftPostsRequest : public sdc::GenericRequest<sdc::Posts> {
 public:
  typedef boost::shared_ptr<SwiftPostsRequest> Ref;

  void HandleRequest(sdc::Connection* connection);
  void HandleResponse(Swift::Payload::ref payload, Swift::ErrorPayload::ref error);
};


#endif /* SWIFT_COMMANDS_H_ */
