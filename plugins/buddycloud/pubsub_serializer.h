/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef PUBSUB_PAYLOAD_SERIALIZER_H_
#define PUBSUB_PAYLOAD_SERIALIZER_H_

#include "Swiften/Serializer/GenericPayloadSerializer.h"

class Atom;
class PubsubConfigureNodeRequest;
class PubsubItemsRequest;
class PubsubPublishRequest;
class PubsubRetractRequest;
class PubsubSubscribeRequest;
class PubsubUnsubscribeRequest;
class Rsm;

/*
 * VARIOUS SERIALIZERS
 */

class AtomSerializer : public Swift::GenericPayloadSerializer<Atom> {
 public:
  std::string serializePayload(boost::shared_ptr<Atom> payload) const;
};

class PubsubItemsRequestSerializer : public Swift::GenericPayloadSerializer<PubsubItemsRequest> {
 public:
  virtual std::string serializePayload(boost::shared_ptr<PubsubItemsRequest> payload) const;
};

class PubsubRetractRequestSerializer : public Swift::GenericPayloadSerializer<PubsubRetractRequest> {
 public:
  virtual std::string serializePayload(boost::shared_ptr<PubsubRetractRequest> payload) const;
};

class PubsubPublishRequestSerializer : public Swift::GenericPayloadSerializer<PubsubPublishRequest> {
 public:
  virtual std::string serializePayload(boost::shared_ptr<PubsubPublishRequest> payload) const;
};

class PubsubSubscribeRequestSerializer : public Swift::GenericPayloadSerializer<PubsubSubscribeRequest> {
 public:
  virtual std::string serializePayload(boost::shared_ptr<PubsubSubscribeRequest> payload) const;
};

class PubsubUnsubscribeRequestSerializer : public Swift::GenericPayloadSerializer<PubsubUnsubscribeRequest> {
 public:
  virtual std::string serializePayload(boost::shared_ptr<PubsubUnsubscribeRequest> payload) const;
};

class PubsubConfigureNodeRequestSerializer : public Swift::GenericPayloadSerializer<PubsubConfigureNodeRequest> {
 public:
  virtual std::string serializePayload(boost::shared_ptr<PubsubConfigureNodeRequest> payload) const;
};

class RsmSerializer : public Swift::GenericPayloadSerializer<Rsm> {
 public:
  virtual std::string serializePayload(boost::shared_ptr<Rsm> payload) const;
};

#endif // PUBSUB_PAYLOAD_SERIALIZER_H_
