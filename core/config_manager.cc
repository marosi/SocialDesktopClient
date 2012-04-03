/*
 * config_manager.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Aug 6, 2011
 */

#include "config_manager.h"
#include "plugin_manager.h"
#include "boost/foreach.hpp"
#include <fstream>
#include <string>

namespace sdc {

const std::string ConfigManager::kConfFile = "social_desktop_client.conf";

template std::map<PluginSignature, Service*> PluginManager::CreateAllInstances<Service>(const PluginType);

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

  // Load Services
  services_ = g_plugin_manager->CreateAllInstances<Service>(SERVICE); // TODO: Change method of getting instances from plugin manager
  std::map<PluginSignature, Service*>::iterator it = services_.begin();
  for(it = services_.begin(); it != services_.end(); ++it) {
    LOG(TRACE) << "Services class '" << it->first << "' loaded successfuly.";
  }

  // Map service object with loaded accounts
  BOOST_FOREACH(Account &account, accounts_) {
    Service* service = services_[account.GetServiceSignature()];
    account.SetService(service);
  }
}


/*
 * TODO: DEPRECATED @{
 */
void ConfigManager::LoadConfig(const std::string & file_path) {
/*  LOG(INFO) << "Loading configuration...";
  //TODO: Hide implementation of file parsing code.
  std::ifstream conf_file(file_path.c_str());
  std::string line;
  bool is_next = true;
  Service::UserConfig* user_conf;
  while(conf_file.good()) {
    std::getline(conf_file, line);
    if(!line.empty() && is_next) { //first line of the user configuration text block
      user_conf = this->GetService(line)->CreateUserConfig(); // FIXME: situation when service ID in configuration doesn't exist
      connection_configs_.push_back(user_conf);
      is_next = false;
    }
    else if(!line.empty() && !is_next) { //on each next line there is a property representation in form of key=value
      std::size_t pos = line.find('=');
      std::string key = line.substr(0, pos);
      std::string value = line.substr(pos + 1);
      user_conf->SetTwo(key, value);
    }
    else if(line.empty()) //user data block finished, next one is coming probably
      is_next = true;
  }
  conf_file.close();*/
}

std::vector<Service::UserConfig*> ConfigManager::GetConnectionConfigs() {
  return connection_configs_;
}

Service* ConfigManager::GetService(const PluginSignature &signature) {
  return services_[signature];
}
/*
 * @}
 */

void ConfigManager::OnExit() {
  std::ofstream file(kConfFile.c_str());
  {
    boost::archive::xml_oarchive archive(file);
    this->serialize(archive, 0);
  }
  file.close();
}

}
