/**
 * @file bc_contact.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 24, 2012
 */

#ifndef BC_CONTACT_H_
#define BC_CONTACT_H_

#include "sdc.h"

class BuddycloudBot;

class BcContact : public sdc::Contact {
 public:
  typedef boost::shared_ptr<BcContact> Ref;

  void SetBot(BuddycloudBot* bot) {
    bot_ = bot;
  }

  BuddycloudBot* bot() {
    return bot_;
  }

 private:
  BuddycloudBot* bot_;
};

#endif /* BC_CONTACT_H_ */
