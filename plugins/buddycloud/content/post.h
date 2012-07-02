/**
 * @file post.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 23, 2012
 */

#ifndef POST_H_
#define POST_H_

#include "sdc.h"

class ChannelController;

class Post1 : public sdc::Post {
 public:
  typedef boost::shared_ptr<Post1> Ref;

  void SetController(ChannelController* controller) {
    controller_ = controller;
  }

  ChannelController* controller() {
    return controller_;
  }

 private:
  ChannelController* controller_;
};

#endif /* POST_H_ */
