/**
 * @file buddycloud_service.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 7, 2012
 */

#ifndef BUDDYCLOUD_SERVICE_H_
#define BUDDYCLOUD_SERVICE_H_

#include "sdc.h"

class BuddycloudService : public sdc::Service {
 public:
  sdc::Connection* CreateConnection();
  sdc::Service::UserConfig* CreateUserConfig();
  sdc::ServiceModelRef CreateMainModel();
  sdc::ServiceViewRef CreateMainView();
  sdc::ServiceController* CreateServiceController();
  void InitializeGui(sdc::MainView* main_view);
};

class BuddycloudUserConfig : public sdc::Service::UserConfig {
 public:
  BuddycloudUserConfig(sdc::Service* service) : sdc::Service::UserConfig(service) { }
};

#endif /* BUDDYCLOUD_SERVICE_H_ */
