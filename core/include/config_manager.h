/**
 * @file config_manager.h
 * @brief SDC configuration interface. User specific configuration data.
 *
 * @author Maros Kasinec
 *   @date Aug 6, 2011
 */

#ifndef CORE_CONFIGMANAGER_H_
#define CORE_CONFIGMANAGER_H_

#include "common.h"

#include "service.h"

#include <iostream>
#include <vector>
#include <map>

namespace sdc {

/// @class ConfigManager
/// @brief Maintains application configuration and provides interface for other managers.
class ConfigManager {
 public:
  void Init();
  void LoadConfig(const std::string &);
  std::vector<Service::UserConfig*> GetConnectionConfigs();

  //void RegisterConnection(ConnectionRegistration*); //HAHA 111st design approach
 private:
  Service* GetService(const PluginSignature &/*signature*/);

  // TODO: ConfigParser
  // TODO: File serializer
  std::map<PluginSignature, Service*> services_; /// Pluged-in services and their configuration options
  std::vector<Service::UserConfig*> connection_configs_; /// Specific user account/service configuration
};

}
#endif /* CORE_CONFIGMANAGER_H_ */
