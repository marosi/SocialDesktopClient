/*
 * config_manager.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Aug 6, 2011
 */

#include "config_manager.h"
#include "account_data.h"
#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include "boost/foreach.hpp"
#include <fstream>

namespace sdc {

const std::string ConfigManager::kConfFile = "sdc.xml";

void ConfigManager::Init() {
  // Load configuration
  std::ifstream file(kConfFile.c_str());
  if (!file.is_open()) {
    LOG(ERROR) << "Configuration file: social_desktop_client.conf is not present.";
    core()->Exit(); // TODO: Exit does nothing so far, this error should be displayed to user
    return;
  }
  {
    boost::archive::xml_iarchive archive(file);
    this->serialize(archive, 0);
  }
  file.close();

  for (unsigned int i = 0; i < accounts_.size(); ++i) {
    accounts_[i]->SetDataIndex(i);
  }
}

void ConfigManager::OnExit() {
  std::ofstream file(kConfFile.c_str());
  {
    boost::archive::xml_oarchive archive(file);
    this->serialize(archive, 0);
  }
  file.close();
}

}
