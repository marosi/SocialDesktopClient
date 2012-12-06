/**
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 *
 *  @file main.cpp
 *
 *  OSW plugin main site of plugin implementation.
 *  It contains neccessary functions for the plugin to be registered in SDC Core.
 *
 */

#include "buddycloud_service.h"
#include "sdc/core/plugin_manager.h"

extern "C" {

sdc::Registration* registerLibrary() {
  std::string name = "Buddycloud";
  std::string version = "0.1";
  std::string description = "Buddycloud protocol plugin";
  sdc::Registration *reg = new sdc::Registration(name, version, description);

  reg->AddClass(sdc::SERVICE, "BuddycloudService", "createBuddycloudService");

  return reg;
}


sdc::Service* createBuddycloudService() {
  return new BcService();
}

}
