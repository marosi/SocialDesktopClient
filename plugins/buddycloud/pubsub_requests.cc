/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "pubsub_requests.h"

GetPubsubItemsRequest::ref GetPubsubItemsRequest::create(const std::string &reciever, const std::string &node, Swift::IQRouter* router) {
  PubsubItemsRequest::ref payload(new PubsubItemsRequest);
  payload->setNode(node);
  return ref(new GetPubsubItemsRequest(Swift::IQ::Get, payload, reciever, router));
}

GetPubsubItemsRequest::ref GetPubsubItemsRequest::create(const std::string &max, const std::string &node, const std::string &reciever, Swift::IQRouter* router) {
  Rsm::ref rsm(new Rsm);
  rsm->setMax(max);
  PubsubItemsRequest::ref payload(new PubsubItemsRequest);
  payload->setNode(node);
  payload->setRsm(rsm);
  return ref(new GetPubsubItemsRequest(Swift::IQ::Get, payload, reciever, router));
}

GetPubsubItemsRequest::ref GetPubsubItemsRequest::create(const std::string &after, const std::string &max, const std::string &node, const std::string &reciever, Swift::IQRouter* router) {
  Rsm::ref rsm(new Rsm);
  rsm->setMax(max);
  rsm->setAfter(after);
  PubsubItemsRequest::ref payload(new PubsubItemsRequest);
  payload->setNode(node);
  payload->setRsm(rsm);
  return ref(new GetPubsubItemsRequest(Swift::IQ::Get, payload, reciever, router));
}
