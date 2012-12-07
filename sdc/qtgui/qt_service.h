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

/**
 * Qt Service class.
 * It contains required elements for a service to be used by Qt GUI.
 */
class QtService : public Service {
 public:
  /**
   * Factory method for new account widget.
   * @return new account widget
   */
  virtual NewAccountWidget* CreateNewAccountPane()=0;
  /**
   * Factory method for new contact widget.
   * @return new contact widget
   */
  virtual NewContactWidget* CreateNewContactWidget()=0;

  virtual std::vector<WidgetFactory*> CreateWidgetFactories()=0;
  /**
   * Factory method for service presenter instance.
   * @return service presenter
   */
  virtual ServicePresenter* CreateServicePresenter()=0;
};

} // namespace sdc

#endif // QT_SERVICE_H_
