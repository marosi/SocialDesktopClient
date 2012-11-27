/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef PUBSUB_PARSER_FACTORIES_H_
#define PUBSUB_PARSER_FACTORIES_H_

#include "Swiften/Parser/PayloadParserFactory.h"
#include "Swiften/Parser/GenericPayloadParserFactory.h"

class AtomParser;

class AnyParserFactory : public Swift::PayloadParserFactory {
 public:
  AnyParserFactory(Swift::PayloadParser* parser) : parser_(parser) {}
  ~AnyParserFactory() { delete parser_; }

  virtual bool canParse(const std::string& /*element*/, const std::string& /*ns*/, const Swift::AttributeMap& /*attributes*/) const {
      return true;
  }

  virtual Swift::PayloadParser* createPayloadParser() {
    return parser_;
  }

 private:
  Swift::PayloadParser* parser_;
};

class AtomParserFactory : public Swift::GenericPayloadParserFactory<AtomParser> {
 public:
  AtomParserFactory() : Swift::GenericPayloadParserFactory<AtomParser>("entry", "http://www.w3.org/2005/Atom") {}
};

#endif // PUBSUB_PARSER_FACTORIES_H_
