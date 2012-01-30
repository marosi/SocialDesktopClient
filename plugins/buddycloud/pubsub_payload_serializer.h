/**
 * @file pubsub_payload_serializer.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Dec 30, 2011
 */

#ifndef PUBSUB_PAYLOAD_SERIALIZER_H_
#define PUBSUB_PAYLOAD_SERIALIZER_H_

#include "Swiften/Swiften.h"
#include "pubsub_payload.h"

class PubsubPayloadSerializer : public Swift::GenericPayloadSerializer<PubsubPayload> {
 public:
  std::string serializePayload(boost::shared_ptr<PubsubPayload> payload) const {
    Swift::XMLElement element("pubsub", "http://jabber.org/protocol/pubsub");
    element.addNode(Swift::XMLTextNode::ref(new Swift::XMLTextNode(payload->getMessage())));
    return element.serialize();
  }
};

#endif /* PUBSUB_PAYLOAD_SERIALIZER_H_ */
