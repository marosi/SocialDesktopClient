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

#include "sdc/qtgui/qt_service.h"

class BuddycloudService : public sdc::QtService {
 public:
  BuddycloudService() {
    SetName("Buddycloud");
    SetDescription("Buddycloud channels service.");
    SetDir("resources/buddycloud");
  }

  sdc::ServiceModel* CreateServiceModel(sdc::Account* account_data);
  sdc::ServicePresenter* CreateServicePresenter();

  sdc::NewAccountWidget* CreateNewAccountPane();
  sdc::NewContactWidget* CreateNewContactWidget();

  std::vector<sdc::WidgetFactory*> CreateWidgetFactories();
};

#endif /* BUDDYCLOUD_SERVICE_H_ */
