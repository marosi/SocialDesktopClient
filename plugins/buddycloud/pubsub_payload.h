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

#include "Swiften/Swiften.h"
#include "boost/shared_ptr.hpp"
#include <string>
#include <vector>

class PubsubPayload : public Swift::Payload {
 public:
  typedef boost::shared_ptr<PubsubPayload> ref;

  enum Type {
    PUBLISH
  };

  PubsubPayload(Type type, std::string node) : type_(type), node_(node) {}

  const std::string& getMessage() { return message_; }
  void setMessage(const std::string& message) { message_ = message; }
  std::string getNode() { return node_; }
  void addItem(Payload::ref item) { items_.push_back(item); }
  std::vector<Payload::ref>& getItems() { return items_; };

  bool isType(Type type) {
    if(type_ == type)
      return true;
    return false;
  }

 private:
  Type type_;
  std::string message_;
  std::string node_;
  std::vector<Payload::ref> items_;
};

#endif /* PUBSUB_PAYLOAD_H_ */
