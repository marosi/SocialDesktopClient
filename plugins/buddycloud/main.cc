/**
 * @file main.cpp
 * @brief OSW plugin main site of plugin implementation.
 *
 * It contains neccessary functions for the plugin to be registered in SDC Core.
 *
 *  Created on: Jul 13, 2011
 *      Author: Maros Kasinec
 */

#include "buddycloud_service.h"

extern "C" {

sdc::Registration* registerLibrary() {
  std::string name = "Buddycloud";
  std::string version = "0.1";
  std::string description = "Buddycloud protocol plugin";
  sdc::Registration *reg = new sdc::Registration(name, version, description);

  reg->AddClass(sdc::SERVICE, "BuddycloudService", "createBuddycloudService");
  //reg->AddClass(sdc::UI, "whatever", "initModelAndController");

  return reg;
}


sdc::Service* createBuddycloudService() {
  return new BuddycloudService();
}

}
