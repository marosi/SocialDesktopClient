/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef QT_SERVICE_H_
#define QT_SERVICE_H_


#include "contact.h"
#include "service.h"
#include <QWidget>

namespace sdc {

class AccountButton;
class NewAccountWidget;
class NewContactWidget;
class ContactWidget;
class QtServiceModel;
class WidgetFactory;
class ServicePresenter;

class QtService : public Service {
 public:
  virtual NewAccountWidget* CreateNewAccountPane()=0;
  virtual NewContactWidget* CreateNewContactWidget()=0;

  virtual std::vector<WidgetFactory*> CreateWidgetFactories()=0;
  virtual ServicePresenter* CreateServicePresenter()=0;
};

} // namespace sdc

#endif // QT_SERVICE_H_
