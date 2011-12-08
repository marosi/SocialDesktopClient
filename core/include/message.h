/**
 * @file message.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Nov 28, 2011
 */

#ifndef CORE_MESSAGE_H_
#define CORE_MESSAGE_H_

namespace sdc {

/**
 * Basic core element carrying social network data.
 */
class Message {
 public:
  virtual void Process()=0;
};

} /* namespace sdc */

#endif /* CORE_MESSAGE_H_ */
