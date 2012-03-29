/**
 * @file pubsub_payload_parser.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Dec 30, 2011
 */

#ifndef PUBSUB_PAYLOAD_PARSER_H_
#define PUBSUB_PAYLOAD_PARSER_H_

#include "sdc.h"
#include "Swiften/Swiften.h"
#include "pubsub.h"

#include <string>

template<class Payload>
class ItemParser : public Swift::GenericPayloadParser<Items<Payload> > {
 public:
  ItemParser() : level_(TopLevel), curr_parser_(0) {}

  ~ItemParser() {
    std::vector<Swift::PayloadParserFactory*>::iterator it;
    for(it = factories_.begin(); it != factories_.end(); ++it) {
      delete *it;
    }
    delete curr_parser_;
  }

  void addPaylodParserFactory(Swift::PayloadParserFactory* parser) {
    parsers_.addFactory(parser);
    factories_.push_back(parser);
  }

  virtual void handleStartElement(const std::string& element, const std::string& ns, const Swift::AttributeMap& attributes) {
    LOG(DEBUG) << "items parser: " << element;
    if (level_ == TopLevel) { // "item" level
    } else {
      if (!curr_parser_) {
        Swift::PayloadParserFactory* factory = parsers_.getPayloadParserFactory(element, ns, attributes);
        assert(factory);
        curr_parser_ = factory->createPayloadParser(); // FIX: each item, new parser is created, is that necessary for ItemParser?
      }
      assert(curr_parser_);
      curr_parser_->handleStartElement(element, ns, attributes);
    }
    ++level_;
  }

  using Swift::GenericPayloadParser<Items<Payload> >::getPayloadInternal;
  virtual void handleEndElement(const std::string& element, const std::string& ns) {
    --level_;
    if (level_ == TopLevel && element == "item") {
      // push back handled payload in item element
      assert(curr_parser_);

      getPayloadInternal()->appendPayload(boost::dynamic_pointer_cast<Payload>(curr_parser_->getPayload())); // FIX: not efficient
      LOG(DEBUG4) << "@@@@@@@@@ adding payload to items";
      // reset variables
      delete curr_parser_;
      curr_parser_ = 0;
    } else {
      assert(curr_parser_);
      curr_parser_->handleEndElement(element, ns);
    }
  }

  virtual void handleCharacterData(const std::string& data) {
    if (level_ > TopLevel) {
      assert(curr_parser_);
      curr_parser_->handleCharacterData(data);
    }
    else { // everything else goes to sink
      character_data_ += data;
    }
  }

 private:
  enum Level {
    TopLevel = 0
  };
  int level_;
  Swift::PayloadParserFactoryCollection parsers_;
  std::vector<Swift::PayloadParserFactory*> factories_;
  Swift::PayloadParser* curr_parser_;
  std::string character_data_;
};

class LogParser : public Swift::PayloadParser {
 public:
  void handleStartElement(const std::string&  element, const std::string&  ns, const Swift::AttributeMap&  attributes) {
    LOG(DEBUG) << "<" << element << " xmlns=" << ns << ">";
  }

  void handleEndElement(const std::string&  element, const std::string&  ns) {
    LOG(DEBUG) << "</" << element << " xmlns=" << ns << ">";
  }

  void handleCharacterData(const std::string&  data) {
    LOG(DEBUG) << data;
  }

 private:
  virtual boost::shared_ptr<Swift::Payload> getPayload() const {
    /* NO SINGLE PAYLOAD */
    return boost::shared_ptr<Swift::Payload>();
  }
};

class AnyParserFactory : public Swift::PayloadParserFactory {
 public:
  AnyParserFactory(Swift::PayloadParser* parser) : parser_(parser) {}
  ~AnyParserFactory() { delete parser_; }

  virtual bool canParse(const std::string& element, const std::string& ns, const Swift::AttributeMap& attributes) const {
      return true;
  }

  virtual Swift::PayloadParser* createPayloadParser() {
    return parser_;
  }

 private:
  Swift::PayloadParser* parser_;
};

class AtomParser : public Swift::GenericPayloadParser<Atom> {
 public:
  AtomParser() : level_(EntryLevel), meta_(NIL) {}

  void handleStartElement(const std::string&  element, const std::string&  ns, const Swift::AttributeMap&  attributes) {
    if (level_ == EntryLevel) {// TopLevel element
    } else if (level_ == MetaLevel) {
      LOG(DEBUG2) << "<" << element << ">";
      if (element == "author") {
        meta_ = Author;
      } else if (element == "content") {
        meta_ = Content;
        inner_text_.clear();
      } else if (element == "object" && ns == "http://activitystrea.ms/spec/1.0/") {
        meta_ = Object;
      } else if (element == "id") {
        meta_ = Id;
        inner_text_.clear();
      }
    } else if (level_ > MetaLevel) {
      if (meta_ == Author) {
        if (element == "name" || element == "jid" || element == "uri")
          inner_text_.clear();
      } else if (meta_ == Object) {
        if (element == "object-type") {
          inner_text_.clear();
        }
      }
    }
    ++level_;
  }

  void handleEndElement(const std::string&  element, const std::string&  ns) {
    --level_;
    if (level_ == MetaLevel) {
      if (meta_ == Content) {
        getPayloadInternal()->setContent(inner_text_);
        meta_ = NIL;
      } else if (meta_ == Id) {
        getPayloadInternal()->setID(inner_text_);
        meta_ = NIL;
      }
    } else if (level_ > MetaLevel) {
      if (meta_ == Author) {
        if (element == "name") {
          getPayloadInternal()->setAuthor(inner_text_);
          meta_ = NIL;
        }
        else if (element == "jid")
          ; // nothing so far
        else if (element == "uri")
          ; // nothing so far
      } else if (meta_ == Object) {
        if (element == "object-type") {
          if (inner_text_ == "note") {
            getPayloadInternal()->setObjectType(Atom::NOTE);
          } else if (inner_text_ == "comment") {
            getPayloadInternal()->setObjectType(Atom::COMMENT);
          }
        }
      }
    }
  }

  void handleCharacterData(const std::string&  data) {
    if (level_ > MetaLevel) {
      inner_text_ += data;
    }
  }

 private:
  enum Level {
    EntryLevel = 0,
    MetaLevel = 1
  };
  enum Meta {
    NIL,
    Author,
    Content,
    Object,
    Id
  };
  int level_;
  int meta_;
  std::string inner_text_;
};

class AtomParserFactory : public Swift::GenericPayloadParserFactory<AtomParser> {
 public:
  AtomParserFactory() : Swift::GenericPayloadParserFactory<AtomParser>("entry", "http://www.w3.org/2005/Atom") {}
};

class PubsubItemsRequestParser : public Swift::GenericPayloadParser<PubsubItemsRequest> {
 public:
  PubsubItemsRequestParser() : level_(TopLevel), is_parsing_items_(false) {
    item_parser_.addPaylodParserFactory(new AtomParserFactory);
  }

  virtual void handleStartElement(const std::string& element, const std::string& ns, const Swift::AttributeMap& attributes) {
    if (level_ == TopLevel) {
      if (element == "items") {
        is_parsing_items_ = true;
      }
    } else {
      if (is_parsing_items_) {
        item_parser_.handleStartElement(element, ns, attributes);
      }
    }
    ++level_;
  }

  virtual void handleEndElement(const std::string& element, const std::string& ns) {
    --level_;
    if (level_ == TopLevel) {
      if (element == "items") {
        getPayloadInternal()->setItems(item_parser_.getPayloadInternal());
        is_parsing_items_ = false;
      }
    } else if (level_ > TopLevel && is_parsing_items_) {
      item_parser_.handleEndElement(element, ns);
    }
  }

  virtual void handleCharacterData(const std::string& data) {
    if (level_ > TopLevel && is_parsing_items_) {
      item_parser_.handleCharacterData(data);
    }
  }

 private:
  enum Level {
    TopLevel = 0
  };
  int level_;
  bool is_parsing_items_;
  ItemParser<Atom> item_parser_;
};

class PubsubParser : public Swift::PayloadParser {
 public:
  PubsubParser() : parser_(0), level_(TopLevel) {}

  ~PubsubParser() {
    delete parser_;
  }

  void handleStartElement(const std::string&  element, const std::string&  ns, const Swift::AttributeMap&  attributes) {
    if (level_ == TopLevel) {
    } else {
      if(!parser_) {
        LOG(DEBUG) << element;
        if (element == "items") {
          type_ = Pubsub::ITEMS;
          parser_ = new PubsubItemsRequestParser;
        } else { // TODO: parse publish, subscribe, unsubscribe, retract responses
          parser_ = new LogParser;
        }
      }
      assert(parser_);
      parser_->handleStartElement(element, ns, attributes);
    }
    ++level_;
  }

  void handleEndElement(const std::string&  element, const std::string&  ns) {
    --level_;
    if (parser_) {
      parser_->handleEndElement(element, ns);
    }
  }

  void handleCharacterData(const std::string& data) {
    if (parser_) {
      parser_->handleCharacterData(data);
    }
  }

  virtual boost::shared_ptr<Swift::Payload> getPayload() const {
    LOG(DEBUG) << "PubsubParser getPayload() object: " << typeid(*parser_->getPayload().get()).name();
    return parser_->getPayload();
  }

 private:
  enum Level {
    TopLevel = 0,
    PayloadLevel = 1
  };
  Pubsub::Action type_;
  PayloadParser* parser_;
  int level_;
  //boost::shared_ptr<Swift::Payload> payload_;
};

class PubsubParserFactory : public Swift::GenericPayloadParserFactory<PubsubParser> {
 public:
  PubsubParserFactory() : Swift::GenericPayloadParserFactory<PubsubParser>("pubsub", "http://jabber.org/protocol/pubsub") {}
};

#endif /* PUBSUB_PAYLOAD_PARSER_H_ */
