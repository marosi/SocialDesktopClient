/*
 * pubsub_payload_serializer.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Feb 8, 2012
 */

#include "pubsub_serializer.h"
#include "sdc.h"

using namespace Swift;

std::string AtomSerializer::serializePayload(boost::shared_ptr<Atom> payload) const {
  LOG(DEBUG) << "get atom element";
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
  return entry.serialize();
}

std::string PubsubItemsRequestSerializer::serializePayload(boost::shared_ptr<PubsubItemsRequest> payload) const {
  XMLElement pubsub("pubsub", "http://jabber.org/protocol/pubsub");
  XMLElement::ref items(new XMLElement("items"));
  items->setAttribute("node", payload->getNode());
  pubsub.addNode(items);
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
  assert(as.canSerialize(payload->getAtom()));
  std::string text_atom = as.serialize(payload->getAtom());

  XMLElement::ref item_elem(new XMLElement("item"));
  item_elem->addNode(boost::shared_ptr<XMLRawTextNode>(new XMLRawTextNode(text_atom)));
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
