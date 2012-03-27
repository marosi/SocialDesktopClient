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

#include "pubsub.h"
#include "Swiften/Swiften.h"

class AtomSerializer : public Swift::GenericPayloadSerializer<Atom> {
 public:
  std::string serializePayload(boost::shared_ptr<Atom> payload) const;
};

class PubsubSerializer : public Swift::GenericPayloadSerializer<Pubsub> {
 public:
  PubsubSerializer();
  virtual std::string serializePayload(boost::shared_ptr<Pubsub> payload) const;

 private:
  Swift::PayloadSerializerCollection serializers_;
};

class PubsubItemsRequestSerializer : public Swift::GenericPayloadSerializer<PubsubItemsRequest> {
 public:
  virtual std::string serializePayload(boost::shared_ptr<PubsubItemsRequest> payload) const;
};

#endif /* PUBSUB_PAYLOAD_SERIALIZER_H_ */
