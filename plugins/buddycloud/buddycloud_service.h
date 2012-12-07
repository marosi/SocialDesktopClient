/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef BUDDYCLOUD_SERVICE_H_
#define BUDDYCLOUD_SERVICE_H_

#include "sdc/qtgui/qt_service.h"

/**
 * buddycloud service class.
 */
class BcService : public sdc::QtService {
 public:
  BcService() {
    SetName("buddycloud");
    SetDescription("buddycloud Channels Service.");
    SetDir("resources/buddycloud");
  }

  sdc::ServiceModel* CreateServiceModel(sdc::Account* account_data);
  sdc::ServicePresenter* CreateServicePresenter();

  sdc::NewAccountWidget* CreateNewAccountPane();
  sdc::NewContactWidget* CreateNewContactWidget();

  std::vector<sdc::WidgetFactory*> CreateWidgetFactories();
};

#endif // BUDDYCLOUD_SERVICE_H_
