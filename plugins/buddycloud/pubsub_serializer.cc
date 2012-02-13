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

PubsubSerializer::PubsubSerializer() {
  serializers_.addSerializer(new AtomSerializer);
}

std::string PubsubSerializer::serializePayload(boost::shared_ptr<Pubsub> payload) const {
  XMLElement pubsub("pubsub", "http://jabber.org/protocol/pubsub");
  switch (payload->getAction()) {
    case Pubsub::SUBSCRIBE: {
      XMLElement::ref subscribe(new XMLElement("subscribe"));
      subscribe->setAttribute("node", payload->getNode());
      pubsub.addNode(subscribe);
      break;
    }
    case Pubsub::UNSUBSCRIBE: {
      XMLElement::ref unsubscribe(new XMLElement("unsubscribe"));
      unsubscribe->setAttribute("node", payload->getNode());
      pubsub.addNode(unsubscribe);
      break;
    }
    case Pubsub::PUBLISH: {
      XMLElement::ref publish(new XMLElement("publish"));
      publish->setAttribute("node", payload->getNode());
      Payload::ref item = payload->getPublishItem();
      if(item) {
        PayloadSerializer* serializer =  serializers_.getPayloadSerializer(item);
        assert(serializer);
        XMLElement::ref item_elem(new XMLElement("item"));
        item_elem->addNode(boost::shared_ptr<XMLRawTextNode>(new XMLRawTextNode(serializer->serialize(item))));
        publish->addNode(item_elem);
      }
      pubsub.addNode(publish);
      break;
    }
    case Pubsub::RETRACT: {
      XMLElement::ref retract(new XMLElement("retract"));
      retract->setAttribute("node", payload->getNode());
      pubsub.addNode(retract);
      break;
    }
    case Pubsub::ITEMS: {
      XMLElement::ref items(new XMLElement("items"));
      items->setAttribute("node", payload->getNode());
      pubsub.addNode(items);
      break;
    }
    default:
      assert(false);
  }
  return pubsub.serialize();
}
