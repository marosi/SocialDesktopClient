/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef PUBSUB_REQUESTS_H_
#define PUBSUB_REQUESTS_H_

#include "payloads/pubsub.h"
#include "Swiften/Queries/GenericRequest.h"

/**
 * Pubsub <items> request class
 */
class GetPubsubItemsRequest : public Swift::GenericRequest<PubsubItemsRequest> {
 public:
  typedef boost::shared_ptr<GetPubsubItemsRequest> ref;

  /**
   * Creates <iq type=get/> stanza with pubsub items request for the specified node.
   *
   * @param node <items/> 'node' attribute
   * @param reciever <iq/> 'to' attribute
   * @param router IQRouter
   * @return boost::shared_ptr<GetPubsubItemsRequest>
   */
  static ref create(const std::string &node, const std::string &reciever, Swift::IQRouter* router) {
    PubsubItemsRequest::ref payload(new PubsubItemsRequest);
    payload->setNode(node);
    return ref(new GetPubsubItemsRequest(Swift::IQ::Get, payload, reciever, router));
  }

  /**
   * Creates <iq type=get/> stanza with pubsub items request for the specified node
   * and limited by the 'max' number of items.
   *
   * @param max maximum number of returned items
   * @param node <items/> 'node' attribute
   * @param reciever <iq/> 'to' attribute
   * @param router IQRouter
   * @return boost::shared_ptr<GetPubsubItemsRequest>
   */
  static ref create(const std::string &max, const std::string &node, const std::string &reciever, Swift::IQRouter* router) {
    Rsm::ref rsm(new Rsm);
    rsm->setMax(max);
    PubsubItemsRequest::ref payload(new PubsubItemsRequest);
    payload->setNode(node);
    payload->setRsm(rsm);
    return ref(new GetPubsubItemsRequest(Swift::IQ::Get, payload, reciever, router));
  }

  /**
   * Creates <iq type=get/> stanza with pubsub items request for the specified node,
   * limited by the 'max' number of items and positioned after a specific item ID.
   *
   * @param after specific item ID in the sequence
   * @param max maximum number of returned items
   * @param node <items/> 'node' attribute
   * @param reciever <iq/> 'to' attribute
   * @param router IQRouter
   * @return boost::shared_ptr<GetPubsubItemsRequest>
   */
  static ref create(const std::string &after, const std::string &max, const std::string &node, const std::string &reciever, Swift::IQRouter* router) {
    Rsm::ref rsm(new Rsm);
    rsm->setMax(max);
    rsm->setAfter(after);
    PubsubItemsRequest::ref payload(new PubsubItemsRequest);
    payload->setNode(node);
    payload->setRsm(rsm);
    return ref(new GetPubsubItemsRequest(Swift::IQ::Get, payload, reciever, router));
  }

  /*virtual void handleResponse(boost::shared_ptr<Swift::Payload> payload, Swift::ErrorPayload::ref error) {
    LOG(DEBUG) << "GetPubsubItemsRequest response object: " << typeid(*payload.get()).name();
    Swift::GenericRequest<PubsubItemsRequest>::handleResponse(payload, error);
  }*/

 private:
  GetPubsubItemsRequest(Swift::IQ::Type type, Swift::Payload::ref payload, const std::string &reciever, Swift::IQRouter* router)
      : Swift::GenericRequest<PubsubItemsRequest>(type, reciever, payload, router) {}
};

/**
 * Pubsub node item <retract> request
 */
class SetPubsubRetractRequest : public Swift::GenericRequest<PubsubRetractRequest> {
 public:
  typedef boost::shared_ptr<SetPubsubRetractRequest> ref;

  static ref create(const std::string &id, const std::string &node, const std::string &reciever, Swift::IQRouter* router) {
    PubsubRetractRequest::ref payload(new PubsubRetractRequest);
    payload->setNode(node);
    payload->setItemID(id);
    return ref(new SetPubsubRetractRequest(payload, reciever, router));
  }

 private:
  SetPubsubRetractRequest(Swift::Payload::ref payload, const std::string &reciever, Swift::IQRouter* router)
        : Swift::GenericRequest<PubsubRetractRequest>(Swift::IQ::Set, reciever, payload, router) {}
};

/**
 * Pubsub node item <publish> request
 */
class SetPubsubPublishRequest : public Swift::GenericRequest<PubsubPublishRequest> {
 public:
  typedef boost::shared_ptr<SetPubsubPublishRequest> ref;

  static ref create(Swift::Payload::ref payload, const Swift::JID &reciever, Swift::IQRouter* router) {
    return ref(new SetPubsubPublishRequest(payload, reciever, router));
  }

 private:
  SetPubsubPublishRequest(Swift::Payload::ref payload, const Swift::JID &reciever, Swift::IQRouter* router)
        : Swift::GenericRequest<PubsubPublishRequest>(Swift::IQ::Set, reciever, payload, router) {}
};

/**
 * Pubsub node <subscribe> request
 */
class SetPubsubSubscribeRequest : public Swift::GenericRequest<PubsubSubscribeRequest> {
 public:
  typedef boost::shared_ptr<SetPubsubSubscribeRequest> ref;

  /**
   * Creates pubsub subscribe request to node 'node' within service 'service'
   *
   * @param node Pubsub node
   * @param reciever Pubsub service domain
   * @param router IQRouter
   * @return boost::shared_ptr<SetPubsubSubscribeRequest>
   */
  static ref create(const std::string &node, const Swift::JID &service, Swift::IQRouter* router) {
    PubsubSubscribeRequest::ref payload(new PubsubSubscribeRequest);
    //payload->setSubscribersJID(jid_); // required by XEP-0060
    payload->setNode(node);
    return ref(new SetPubsubSubscribeRequest(payload, service, router));
  }

 private:
  SetPubsubSubscribeRequest(Swift::Payload::ref payload, const Swift::JID &reciever, Swift::IQRouter* router)
      : Swift::GenericRequest<PubsubSubscribeRequest>(Swift::IQ::Set, reciever, payload, router) {}
};

/**
 * Pubsub node <unsubscribe> request
 */
class SetPubsubUnsubscribeRequest : public Swift::GenericRequest<PubsubUnsubscribeRequest> {
 public:
  typedef boost::shared_ptr<SetPubsubUnsubscribeRequest> ref;

  /**
   * Creates pubsub unsubscribe request from node 'node' within service 'service'
   *
   * @param node Pubsub node
   * @param reciever Pubsub service domain
   * @param router IQRouter
   * @return boost::shared_ptr<SetPubsubUnsubscribeRequest>
   */
  static ref create(const std::string &node, const Swift::JID &service, Swift::IQRouter* router) {
    PubsubUnsubscribeRequest::ref payload(new PubsubUnsubscribeRequest);
    payload->setNode(node);
    return ref(new SetPubsubUnsubscribeRequest(payload, service, router));
  }

 private:
  SetPubsubUnsubscribeRequest(Swift::Payload::ref payload, const Swift::JID &reciever, Swift::IQRouter* router)
      : Swift::GenericRequest<PubsubUnsubscribeRequest>(Swift::IQ::Set, reciever, payload, router) {}
};

/**
 * Pubsub node <create><configure/></create> request
 */
class SetPubsubConfigureNodeRequest : public Swift::GenericRequest<PubsubConfigureNodeRequest> {
 public:
  typedef boost::shared_ptr<SetPubsubConfigureNodeRequest> ref;

  static ref create(Swift::Payload::ref payload, const Swift::JID &reciever, Swift::IQRouter* router) {
    return ref(new SetPubsubConfigureNodeRequest(payload, reciever, router));
  }

 private:
  SetPubsubConfigureNodeRequest(Swift::Payload::ref payload, const Swift::JID &reciever, Swift::IQRouter* router)
      : Swift::GenericRequest<PubsubConfigureNodeRequest>(Swift::IQ::Set, reciever, payload, router) {}
};


#endif // PUBSUB_REQUESTS_H_
