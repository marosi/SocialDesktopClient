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

#include "sdc.h"
#include "Swiften/Swiften.h"
#include "boost/shared_ptr.hpp"
#include <string>
#include <vector>

template<class P>
class Items : public Swift::Payload {
 public:
  typedef boost::shared_ptr<Items<P> > ref;

  void appendPayload(boost::shared_ptr<P> payload) {
    items_.push_back(payload);
  }

  const std::vector<boost::shared_ptr<P> >& get() const {
    return items_;
  }

 private:
  std::vector<boost::shared_ptr<P> > items_;
};

/**
 * Various payloads
 */

class Atom : public Swift::Payload {
 public:
  friend class AtomSerializer;
  typedef boost::shared_ptr<Atom> ref;
  enum Verb { POST };
  enum ObjectType { NOTE, COMMENT };

  Atom() {}

  void setID(const std::string & id) {
    id_ = id;
  }

  void setVerb(Verb verb) {
    verb_ = verb;
  }

  void setObjectType(ObjectType type) {
    object_type_ = type;
  }

  void setPublished(const std::string & published) {
    published_ = published;
  }

  void setAuthor(const std::string & author) {
    author_ = author;
  }

  void setContent(const std::string & content) {
    content_ = content;
  }

  std::string getID() const {
    return id_;
  }

  Verb getVerb() const {
    return verb_;
  }

  ObjectType getObjectType() const {
    return object_type_;
  }

  std::string getVerbString() const {
    switch (verb_) {
      case POST:
        return "post";
      default:
        assert(false);
    }
  }

  std::string getObjectTypeString() const {
    switch (object_type_) {
      case NOTE:
        return "note";
      case COMMENT:
        return "comment";
      default:
        assert(false);
    }
  }

  std::string getPublished() const {
    return published_;
  }

  std::string getAuthor() const {
    return author_;
  }

  std::string getContent() const {
    return content_;
  }


 private:
  Verb verb_;
  ObjectType object_type_;
  std::string published_;
  std::string author_;
  std::string content_;
  std::string id_;
};

class Geoloc : Swift::Payload {
  // TODO: As far as this is a desktop client geoloc info are not that important
};

/**
 * Pubsub payloads
 */

class PubsubNode : public Swift::Payload {
 public:
  typedef boost::shared_ptr<PubsubNode> ref;

  void setNode(const std::string &node) {
    node_ = node;
  }

  std::string getNode() {
    return node_;
  }

 private:
  std::string node_;
};

class PubsubNodeItem : public PubsubNode {
 public:
  typedef boost::shared_ptr<PubsubNodeItem> ref;

  void setItemID(const std::string &id) {
    id_ = id;
  }

  std::string getItemID() {
    return id_;
  }

 private:
  std::string id_;
};

/**
 * Pubsub request payloads
 */
class PubsubItemsRequest : public PubsubNode {
 public:
  typedef boost::shared_ptr<PubsubItemsRequest> ref;

  void setItems(Items<Atom>::ref items) {
    items_ = items;
  }

  Items<Atom>::ref getItems() {
    return items_;
  }

 private:
  Items<Atom>::ref items_;
};

class PubsubRetractRequest : public PubsubNodeItem {
 public:
  typedef boost::shared_ptr<PubsubRetractRequest> ref;

  // TODO: this class can also be just a typedef
};

class PubsubPublishRequest : public PubsubNodeItem {
 public:
  typedef boost::shared_ptr<PubsubPublishRequest> ref;

  PubsubPublishRequest() : atom_(new Atom) {}

  void setAtom(Atom::ref atom) {
    atom_ = atom;
  }

  Atom::ref& getAtom() {
    return atom_;
  }

 private:
  Atom::ref atom_;
};

class PubsubSubscribeRequest : public PubsubNode {
 public:
  typedef boost::shared_ptr<PubsubSubscribeRequest> ref;

  PubsubSubscribeRequest() : subscription_(Unset) {}

  enum Subscription { Unset, Subscribed };

  void setSubscribersJID(const Swift::JID &jid) {
    jid_ = jid;
  }

  std::string getSubscribersJID() {
    return jid_.toString();
  }

  void setSubscription(Subscription type) {
    subscription_ = type;
  }

  Subscription getSubscription() {
    return subscription_;
  }

 private:
  Subscription subscription_;
  Swift::JID jid_;
};

class PubsubUnsubscribeRequest : public PubsubNode {

};

/**
 * Event pubsub payload, usually comming to pubsub node subscribers on node update.
 * It is so far identical to PubsubItemsRequest therefore only shared_ptr typedef is added.
 */
class EventPayload : public PubsubItemsRequest {
 public:
  typedef boost::shared_ptr<EventPayload> ref;
};

#endif /* PUBSUB_PAYLOAD_H_ */
