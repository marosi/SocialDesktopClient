/**
 * @file simple_message.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Dec 6, 2011
 */

#ifndef SIMPLE_MESSAGE_H_
#define SIMPLE_MESSAGE_H_

#include "message.h"
#include <string>

namespace sdc {

// TODO: testing ... delete afterwards
class DummyMessage : public Message {
 public:
  DummyMessage(std::string content) : Message(content) {
    content_ = content;
  }
  virtual void OnDispatch()=0;

 protected:
  std::string content_;
};

} /* namespace sdc */

#endif /* SIMPLE_MESSAGE_H_ */
