/**
 * @file model.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 31, 2012
 */

#ifndef B_MODEL_H_
#define B_MODEL_H_

#include "sdc.h"

class BModel : public sdc::ServiceModel {
 public:
  enum OnlineState { OFFLINE, ONLINE };

  BModel() : online_state_(OFFLINE) {}

  sdc::Connection* CreateConnection() {}

  void SetOnlineState(std::string state) {
    if(state == "offline") {
      online_state_ = OFFLINE;
    } else if (state == "online") {
      online_state_ = ONLINE;
    } else {
      assert(false);
    }
  }

  std::string GetOnlineState() {
    switch (online_state_) {
      case OFFLINE:
        return "offline";
      case ONLINE:
        return "online";
      default:
        assert(false);
    }
  }

 private:
  OnlineState online_state_;
};

#endif /* B_MODEL_H_ */
