/**
 * @file common.h
 * @brief This is master header file providing SocialDesktopClient API.
 *	
 * Use this header file in plugin development.
 *
 * @author Maros Kasinec
 *   @date Aug 6, 2011
 */

#ifndef CORE_COMMON_H_
#define CORE_COMMON_H_

#include "log.h"

#include <string>

namespace sdc {

/// Core typedefs
typedef std::string PluginSignature;
typedef std::string LibSignature;

class PluginManager;
class ConnectionManager;
class ConfigManager;
class EventManager;

/// @defgroup manager_glob_vars Managers's global access variables
/// @{
extern ConfigManager* g_config_manager;
extern ConnectionManager* g_connection_manager;
extern PluginManager* g_plugin_manager;
extern EventManager* g_event_manager;
/// @}

}

#endif /* CORE_COMMON_H_ */
