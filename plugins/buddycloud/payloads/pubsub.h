/**
 * @file pubsub.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Dec 30, 2011
 */

#ifndef PUBSUB_PAYLOAD_H_
#define PUBSUB_PAYLOAD_H_

#include "pubsub_node.h"
#include "atom.h"
#include "items.h"
#include "Swiften/JID/JID.h"
#include "Swiften/Elements/Form.h"
#include "boost/shared_ptr.hpp"
#include <string>
#include <vector>

class Geoloc : Swift::Payload {
  // TODO: As far as this is a desktop client geoloc info are not that important
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
};

class PubsubPublishRequest : public PubsubNodeItem {
 public:
  typedef boost::shared_ptr<PubsubPublishRequest> ref;

  PubsubPublishRequest() : payload_() {}

  void setPayload(Swift::Payload::ref atom) {
    payload_ = atom;
  }

  Swift::Payload::ref& getPayload() {
    return payload_;
  }

 private:
  Swift::Payload::ref payload_;
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

class PubsubConfigureNodeRequest : public PubsubNode {
 public:
  typedef boost::shared_ptr<PubsubConfigureNodeRequest> ref;

  void setForm(Swift::Form::ref form) {
    form_ = form;
  }

  Swift::Form::ref getForm() {
    return form_;
  }

 private:
  Swift::Form::ref form_;
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
