/**
 * @file social_desktop_client.h
 * @brief The heart of SocialDesktopClient application.
 *
 * @author Maros Kasinec
 *   @date Jul 8, 2011
 */

#ifndef CORE_SOCIALDESKTOPCLIENT_H_
#define CORE_SOCIALDESKTOPCLIENT_H_

#include "common.h"
#include "content.h"
#include "boost/noncopyable.hpp"
#include "boost/thread.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/signals2.hpp"
#include <vector>
#include <map>
#include <set>

namespace sdc {

class AccountData;
class Service;
class ServiceModel;
class UI;

class PluginManager;
class ConfigManager;
class EventManager;
class ConnectionManager;

/**
 * @class SocialDesktopClient
 * @brief The heart of SocialDesktopClient application.
 */
class Core : boost::noncopyable {
 public:
  typedef std::map<std::string, ServiceModel*> AccountModelsMap;

	Core(int argc, char* argv[]);
	~Core();
	void Start();
	void Exit();

  EventManager* events() {
    return event_manager_;
	}
	ConnectionManager* connections() {
    return connection_manager_;
	}
	ConfigManager* data() {
    return config_manager_;
	}
	PluginManager* plugins() {
    return plugin_manager_;
	}

	std::vector<Service*> services();

	Service* service(const PluginSignature &/* signature */);

	std::vector<ServiceModel*> GetModels() {
	  return service_models_;
	}

	AccountModelsMap models() {
	  return account_models_;
	}

	ServiceModel* model(const std::string &account_id) {
	  return account_models_[account_id];
	}

	int GetReturnCode() { return return_code_; }
	/**
   * Signals
   */
  boost::signals2::signal<void ()>  onGuiPrepared;
  boost::signals2::signal<void (AccountData*)> onAccountActivated;
  boost::signals2::signal<void (AccountData*)> onAccountDeactivated;

 private:
	void Init();
	void Exec();
	void ExecUi();
  /*
   * Accounts
   */
  void ActivateAccount(AccountData* data);
  void DeactivateAccount(AccountData* data);
  /*
   * Managers
   */
  PluginManager* plugin_manager_;
  ConfigManager* config_manager_;
  EventManager* event_manager_;
  ConnectionManager* connection_manager_;
  /*
   * Threading
   */
	boost::thread core_;
	boost::mutex mutex_;
	boost::condition_variable gui_unprepared_;
	bool is_gui_prepared_;
	//boost::thread ui_; // Qt GUI cannot run in secondary thread
  UI* ui_;
	int return_code_;
  /*
   * Models & accounts
   */
  std::vector<ServiceModel*> service_models_;
  AccountModelsMap account_models_;
  std::map<PluginSignature, Service*> services_; /// Pluged-in services and their configuration options

  //std::set<Content::Ref> contents_;
};

}

#endif /* CORE_SOCIALDESKTOPCLIENT_H_ */
