/**
 * @file qt_service.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 14, 2012
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

} /* namespace sdc */

#endif /* QT_SERVICE_H_ */
