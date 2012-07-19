/*
 * pubsub_payload_serializer.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Feb 8, 2012
 */

#include "pubsub_serializer.h"
#include "payloads/pubsub.h"
#include "Swiften/Serializer/PayloadSerializers/FormSerializer.h"
#include "Swiften/Serializer/XML/XMLElement.h"
#include "Swiften/Serializer/XML/XMLRawTextNode.h"
#include "boost/make_shared.hpp"

using namespace Swift;

std::string AtomSerializer::serializePayload(boost::shared_ptr<Atom> payload) const {
  XMLElement entry("entry", "http://www.w3.org/2005/Atom");
  entry.setAttribute("xmlns:activity", "http://activitystrea.ms/spec/1.0/");
//
  XMLElement::ref published(new XMLElement("published", "", "2010-01-06T21:41:32Z"));
  entry.addNode(published);
//
  XMLElement::ref author(new XMLElement("author"));
  {
    XMLElement::ref name(new XMLElement("name", "", payload->getAuthor()));
    author->addNode(name);
    XMLElement::ref jid(new XMLElement("jid", "http://buddycloud.com/atom-elements-0", payload->getAuthor()));
    author->addNode(jid);
  }
  entry.addNode(author);
//
  XMLElement::ref content(new XMLElement("content"));
  content->setAttribute("type", "text");
  content->addNode(boost::shared_ptr<XMLRawTextNode>(new XMLRawTextNode(payload->getContent())));
  entry.addNode(content);
  // XMLElement geoloc("geoloc");

  XMLElement::ref activity_verb(new XMLElement("activity:verb", "", payload->getVerbString()));
  entry.addNode(activity_verb);
//
  XMLElement::ref activity_object(new XMLElement("activity:object"));
  {
    XMLElement::ref activity_object_type(new XMLElement("activity:object-type", "", payload->getObjectTypeString()));
    activity_object->addNode(activity_object_type);
  }
  entry.addNode(activity_object);
//
  if (payload->getInReplyTo() != "") {
    XMLElement::ref in_reply_to(new XMLElement("in-reply-to", "http://purl.org/syndication/thread/1.0"));
    in_reply_to->setAttribute("ref", payload->getInReplyTo());
    entry.addNode(in_reply_to);
  }
  return entry.serialize();
}

std::string PubsubItemsRequestSerializer::serializePayload(boost::shared_ptr<PubsubItemsRequest> payload) const {
  XMLElement pubsub("pubsub", "http://jabber.org/protocol/pubsub");
  XMLElement::ref items(new XMLElement("items"));
  items->setAttribute("node", payload->getNode());
  pubsub.addNode(items);
  if (payload->getRsm()) {
    RsmSerializer rs;
    pubsub.addNode(boost::make_shared<XMLRawTextNode>(rs.serialize(payload->getRsm())));
  }
  return pubsub.serialize();
}

std::string PubsubRetractRequestSerializer::serializePayload(boost::shared_ptr<PubsubRetractRequest> payload) const {
  XMLElement pubsub("pubsub", "http://jabber.org/protocol/pubsub");
  XMLElement::ref retract(new XMLElement("retract"));
  retract->setAttribute("node", payload->getNode());
  retract->setAttribute("notify", "true");
  XMLElement::ref item(new XMLElement("item"));
  item->setAttribute("id", payload->getItemID());
  retract->addNode(item);
  pubsub.addNode(retract);
  return pubsub.serialize();
}

std::string PubsubPublishRequestSerializer::serializePayload(boost::shared_ptr<PubsubPublishRequest> payload) const {
  XMLElement pubsub("pubsub", "http://jabber.org/protocol/pubsub");
  XMLElement::ref publish(new XMLElement("publish"));
  publish->setAttribute("node", payload->getNode());

  AtomSerializer as;
  assert(as.canSerialize(payload->getPayload()));
  std::string text_atom = as.serialize(payload->getPayload());

  XMLElement::ref item_elem(new XMLElement("item"));
  item_elem->addNode(boost::make_shared<XMLRawTextNode>(text_atom));
  publish->addNode(item_elem);

  pubsub.addNode(publish);
  return pubsub.serialize();
}

std::string PubsubSubscribeRequestSerializer::serializePayload(boost::shared_ptr<PubsubSubscribeRequest> payload) const {
  XMLElement pubsub("pubsub", "http://jabber.org/protocol/pubsub");
  XMLElement::ref subscribe(new XMLElement("subscribe"));
  subscribe->setAttribute("node", payload->getNode());
  subscribe->setAttribute("jid", payload->getSubscribersJID());
  pubsub.addNode(subscribe);
  return pubsub.serialize();
}

std::string PubsubUnsubscribeRequestSerializer::serializePayload(boost::shared_ptr<PubsubUnsubscribeRequest> payload) const {
  XMLElement pubsub("pubsub", "http://jabber.org/protocol/pubsub");
  XMLElement::ref unsubscribe(new XMLElement("unsubscribe"));
  unsubscribe->setAttribute("node", payload->getNode());
  pubsub.addNode(unsubscribe);
  return pubsub.serialize();
}

std::string PubsubConfigureNodeRequestSerializer::serializePayload(boost::shared_ptr<PubsubConfigureNodeRequest> payload) const {
  XMLElement pubsub("pubsub", "http://jabber.org/protocol/pubsub");
  XMLElement::ref create(new XMLElement("create"));
  create->setAttribute("node", payload->getNode());
  pubsub.addNode(create);
  FormSerializer fs;
  XMLElement::ref configure(new XMLElement("configure"));
  configure->addNode(boost::make_shared<XMLRawTextNode>(fs.serialize(payload->getForm())));
  pubsub.addNode(configure);
  return pubsub.serialize();
}

/**
 * Serializes RSM payload data.
 *
 * RSM parameters as <first/>, <last/>, <count/> are not implemented in client
 * because they are only showed in responses from server.
 */
std::string RsmSerializer::serializePayload(boost::shared_ptr<Rsm> payload) const {
  XMLElement rsm("set", "http://jabber.org/protocol/rsm");
  XMLElement::ref max(new XMLElement("max", "", payload->getMax()));
  rsm.addNode(max);
  if (!payload->getAfter().empty()) {
    XMLElement::ref after(new XMLElement("after", "", payload->getAfter()));
    rsm.addNode(after);
  }
  if (!payload->getBefore().empty()) {
    XMLElement::ref before(new XMLElement("before", "", payload->getBefore()));
    rsm.addNode(before);
  }
  if (!payload->getIndex().empty()) {
    XMLElement::ref index(new XMLElement("index", "", payload->getIndex()));
    rsm.addNode(index);
  }
  return rsm.serialize();
}
