/**
 * @file pubsub_payload.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Dec 30, 2011
 */

#ifndef PUBSUB_PAYLOAD_H_
#define PUBSUB_PAYLOAD_H_

#include <string>

class PubsubPayload : public Swift::Payload {
 public:
  PubsubPayload() {}
  const std::string& getMessage() {
    return message_;
  }
  void setMessage(const std::string& message) {
    message_ = message;
  }
 private:
  std::string message_;
};

#endif /* PUBSUB_PAYLOAD_H_ */
