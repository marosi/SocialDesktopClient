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
class ServiceManager;

/**
 * @class SocialDesktopClient
 * @brief The heart of SocialDesktopClient application.
 */
class Core : boost::noncopyable {
 public:
  static void Create(UI* ui);
  static Core* Instance();
  static std::string home_dir;

	void Start();
	void Exit();

  EventManager* events() {
    return event_manager_;
  }
  DataManager* data() {
    return config_manager_;
	}
	PluginManager* plugins() {
    return plugin_manager_;
	}

	std::vector<Service*> services();

  Service* service(const PluginSignature &signature);

  std::vector<ServiceModel*> models() {
	  return service_models_;
	}

	ServiceModel* model(const std::string &account_id) {
    return service_models_map_[account_id];
  }

	int GetReturnCode() { return return_code_; }
	/**
   * Signals
   */
  boost::signals2::signal<void ()>  onGuiPrepared;
  boost::signals2::signal<void (const std::string)> onAccountActivated;
  boost::signals2::signal<void (const std::string)> onAccountDeactivated;

 private:
  Core(UI* ui);
  ~Core();
  static Core* instance_;


	void Init();
  void Exec();
	void ExecUi();
  /*
   * Accounts
   */
  void ActivateAccount(Account* data);
  void DeactivateAccount(Account* data);
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
  std::map<PluginSignature, Service*> services_; /// Pluged-in services and their configuration options
};

}

#endif /* CORE_SOCIALDESKTOPCLIENT_H_ */
