/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_SOCIALDESKTOPCLIENT_H_
#define CORE_SOCIALDESKTOPCLIENT_H_

#include "common.h"
#include "data_manager.h"
#include "event_manager.h"
#include "plugin_manager.h"
#include "boost/noncopyable.hpp"
#include "boost/thread.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/signals2.hpp"
#include <map>
#include <set>
#include <string>
#include <vector>

namespace sdc {

class Account;
class Service;
class ServiceModel;
class UI;

class PluginManager;
class DataManager;
class EventManager;

/**
 * The heart of SocialDesktopClient application.
 * Contains top-level application business logic.
 */
class Core : boost::noncopyable {
 public:
  /**
   * Method for creating class singleton instance.
   * @param ui place for
   */
  static void Create(UI* ui);
  /**
   * Singleton getter method.
   * @return singleton Core instance
   */
  static Core* Instance();
  static std::string home_dir; /**< holder for non-trivial home directory */

  /**
   * Core instance execution point.
   */
	void Start();
  /**
   * Terminates all Core components
   */
	void Exit();

  /**
   * EventManager getter.
   */
  EventManager* events() {
    return event_manager_;
  }
  /**
   * DataManager getter.
   */
  DataManager* data() {
    return config_manager_;
	}
  /**
   * PluginManager getter.
   */
	PluginManager* plugins() {
    return plugin_manager_;
	}

  /**
   * Service getter.
   * @return vector of services
   */
	std::vector<Service*> services();

  /**
   * Returns service based on service ID.
   * @param signature service signature
   * @return service instance
   */
  Service* service(const PluginSignature &signature);

  /**
   * Getter for all activated service models.
   * @return vector of service model instances
   */
  std::vector<ServiceModel*> models() {
	  return service_models_;
	}

  /**
   * Returns service model specific by its account.
   * @param account_id ID of an account
   * @return service model instance
   */
	ServiceModel* model(const std::string &account_id) {
    return service_models_map_[account_id];
  }

  /**
   * Return code for main function.
   * @return return code
   */
	int GetReturnCode() { return return_code_; }
  /*
   * Signals
   */
  boost::signals2::signal<void ()>  onGuiPrepared; /**< indicates GUI is initialized */
  boost::signals2::signal<void (const std::string)> onAccountActivated; /**< indicates event of account activation */
  boost::signals2::signal<void (const std::string)> onAccountDeactivated; /**< indicates event of account deactivation */

 private:
  Core(UI* ui);
  ~Core();
  static Core* instance_; /**< actual singleton instance */

  /**
   * Core instance initialization.
   */
	void Init();
  /**
   * Entry point of Core thread execution.
   * Application lives as long as this method is processed.
   */
  void Exec();
  /**
   * Execution point of UI implementation.
   */
	void ExecUi();
  /*
   * Accounts
   */
  /**
   * Activates particular account within applicaiton.
   * @param data account data
   */
  void ActivateAccount(Account* data);
  /**
   * Deactivates particular account within application.
   * @param data account data
   */
  void DeactivateAccount(Account* data);
  /**
   * Service model thread initialization.
   * @param model actual service model
   */
  void MakeServiceThread(ServiceModel* model);
  /*
   * Managers
   */
  PluginManager* plugin_manager_;
  DataManager* config_manager_;
  EventManager* event_manager_;
  /*
   * Threading
   */
  UI* ui_;
	boost::thread core_;
  boost::thread_group model_threads_;
  std::map<ServiceModel*, boost::thread*> model_threads_map_;
	boost::mutex mutex_;
	boost::condition_variable gui_unprepared_;
	bool is_gui_prepared_;
	//boost::thread ui_; // Qt GUI cannot run in secondary thread
	int return_code_;
  /*
   * Models & accounts
   */
  std::vector<ServiceModel*> service_models_;
  std::map<std::string, ServiceModel*> service_models_map_;
  std::map<PluginSignature, Service*> services_; /**< Pluged-in services and their configuration options */
};

} // namespace sdc

#endif // CORE_SOCIALDESKTOPCLIENT_H_
