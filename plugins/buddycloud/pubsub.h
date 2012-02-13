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

class Pubsub : public Swift::Payload {
 public:
  typedef boost::shared_ptr<Pubsub> ref;
  enum Action { SUBSCRIBE, UNSUBSCRIBE, PUBLISH, RETRACT, ITEMS };

  Pubsub(Action type, std::string node) :
    type_(type),
    node_(node) {}

  void setPublishItem(Swift::Payload::ref payload) {
    publish_item_ = payload;
  }

  Swift::Payload::ref getPublishItem() {
    return publish_item_;
  }

  Action getAction() {
    return type_;
  }

  std::string getNode() {
    return node_;
  }

 private:
  Action type_;
  std::string node_;
  Swift::Payload::ref publish_item_;
};

class Atom : public Swift::Payload {
 public:
  friend class AtomSerializer;
  typedef boost::shared_ptr<Atom> ref;
  enum Verb { POST };
  enum ObjectType { NOTE, COMMENT };

  void setVerb(Verb verb) {
    verb_ = verb;
  }

  void setObjectType(ObjectType type) {
    object_type_ = type;
  }

  void setPublished(std::string published) {
    published_ = published;
  }

  void setAuthor(std::string author) {
    author_ = author;
  }

  void setContent(std::string content) {
    content_ = content;
  }

  Verb getVerb() {
    return verb_;
  }

  ObjectType getObjectType() {
    return object_type_;
  }

  std::string getVerbString() {
    switch (verb_) {
      case POST:
        return "post";
      default:
        assert(false);
    }
  }

  std::string getObjectTypeString() {
    switch (object_type_) {
      case NOTE:
        return "note";
      case COMMENT:
        return "comment";
      default:
        assert(false);
    }
  }

  std::string getPublished() {
    return published_;
  }

  std::string getAuthor() {
    return author_;
  }

  std::string getContent() {
    return content_;
  }


 private:
  Verb verb_;
  ObjectType object_type_;
  std::string published_;
  std::string author_;
  std::string content_;
};

class Geoloc : Swift::Payload {
  // TODO: As far as this is a desktop client geoloc info are not that important
};

#endif /* PUBSUB_PAYLOAD_H_ */
