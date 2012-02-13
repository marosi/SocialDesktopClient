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

/*class PubsubPayloadParser : public Swift::GenericPayloadParser<PubsubPayload> {
 public:
  void handleStartElement(
      const std::string&  element , const std::string&  ns , const Swift::AttributeMap&  map ) {
    current_depth_++;
  }

  void handleEndElement(const std::string&  element , const std::string&  ns ) {
    current_depth_--;
    if(current_depth_ == 0) {
      getPayloadInternal()->setMessage(current_text_);
      LOG(DEBUG2) << getPayloadInternal()->getMessage();
    }
  }

  void handleCharacterData(const std::string& data) {
    current_text_ += data;
  }
 private:
  int current_depth_;
  std::string current_text_;
};

class PubsubPayloadParserFactory : public Swift::GenericPayloadParserFactory<PubsubPayloadParser> {
 public:
  PubsubPayloadParserFactory() : Swift::GenericPayloadParserFactory<PubsubPayloadParser>("event", "http://jabber.org/protocol/pubsub#event") {}
};*/

#endif /* PUBSUB_PAYLOAD_PARSER_H_ */
