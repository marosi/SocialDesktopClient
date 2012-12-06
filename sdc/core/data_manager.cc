/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "data_manager.h"
#include "account.h"
#include "core.h"
#include "log.h"
#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"
#include <fstream>

namespace sdc {

const std::string DataManager::kConfFile = "sdc.xml";

/**
 * On DataManager initialization configuration file is read
 * and deserialized into DataManager instance.
 */
void DataManager::Init() {
  // Load configuration
  std::ifstream file(kConfFile);
  if (!file.is_open()) {
    LOG(WARNING) << "Configuration file: sdc.xml is not present.";
    //core()->Exit(); // TODO: Exit does nothing so far, this error should be displayed to user
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

/**
 * Upon DataManager exit all structures are being serialized into an XML archive.
 */
void DataManager::OnExit() {
  std::ofstream file(kConfFile.c_str());
  {
    boost::archive::xml_oarchive archive(file);
    this->serialize(archive, 0);
  }
  file.close();
}

} // namespace sdc
