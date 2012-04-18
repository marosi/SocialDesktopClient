/**
 * @file buddycloud_service_account_model.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 14, 2012
 */

#ifndef BUDDYCLOUD_MODEL_H_
#define BUDDYCLOUD_MODEL_H_

#include "sdc.h"
#include "buddycloud_connection.h"
#include "buddycloud_bot.h"

class BuddycloudModel : public sdc::QtServiceModel {

    Q_OBJECT

 public:
  BuddycloudModel(sdc::AccountData* account)
      : QtServiceModel(account),
        account_(account),
        connection_(0),
        bot_(0) {
    LOG(DEBUG) << "This is buddycloud Service Model!";
  }

  sdc::Connection* CreateConnection() {
    bot_ = new BuddycloudBot(account_->GetUid(), account_->GetPassword());
    connection_ = new BuddycloudConnection(bot_);
    return connection_;
  }

  // TODO: Event loop testing
  void TestEventLoop() {
    //core()->event_manager()->PostEvent(boost::bind(&Core::Process, core(), message));
  }

 signals:

 private:
  sdc::AccountData* account_;
  BuddycloudConnection* connection_; // TODO: Rename to ChannelConnection
  BuddycloudBot* bot_; // TODO: Rename to ChannelBot
};

#endif /* BUDDYCLOUD_MODEL_H_ */
