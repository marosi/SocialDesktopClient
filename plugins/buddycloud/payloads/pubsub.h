/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef PUBSUB_H_
#define PUBSUB_H_

#include "pubsub_node.h"
#include "atom.h"
#include "items.h"
#include "Swiften/JID/JID.h"
#include "Swiften/Elements/Form.h"
#include "boost/shared_ptr.hpp"
#include <string>
#include <vector>

class Geoloc : Swift::Payload {
  //TODO
};

class Rsm : public Swift::Payload {
 public:
  typedef boost::shared_ptr<Rsm> ref;

  void setMax(const std::string &max) { max_ = max; }
  const std::string& getMax() const { return max_; }
  void setAfter(const std::string &after) { after_ = after; }
  const std::string& getAfter() const { return after_; }
  void setBefore(const std::string &before) { before_ = before; }
  const std::string& getBefore() const { return before_; }
  void setIndex(const std::string &index) { index_ = index; }
  const std::string& getIndex() const { return index_; }

  void setFirst(const std::string &first) { first_ = first; }
  const std::string& getFirst() const { return first_; }
  void setFirstIndex(const std::string &index) { first_index_ = index; }
  const std::string& getFirstIndex() { return first_index_; }
  void setLast(const std::string &last) { last_ = last; }
  const std::string& getLast() const { return last_; }
  void setCount(const std::string &count) { count_ = count; }
  const std::string& getCount() const { return count_; }

 private:
  std::string max_;
  std::string after_;
  std::string before_;
  std::string index_;

  std::string first_;
  std::string first_index_;
  std::string last_;
  std::string count_;
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

  void setItems(Items::ref items) {
    items_ = items;
  }

  Items::ref getItems() {
    return items_;
  }

  void setRsm(Rsm::ref rsm) {
    rsm_ = rsm;
  }

  Rsm::ref getRsm() {
    return rsm_;
  }

 private:
  Items::ref items_;
  Rsm::ref rsm_;

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

#endif // PUBSUB_H_
