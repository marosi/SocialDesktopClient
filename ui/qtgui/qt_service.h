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

#include "service.h"
#include "content.h"
#include <QWidget>

namespace sdc {

class AccountButton;
class NewAccountWidget;
class ContactWidget;
class PostWidget;
class QtServiceModel;
class WidgetFactory;

class QtService : public Service {
 public:
  virtual NewAccountWidget* CreateNewAccountPane()=0;
  virtual AccountButton* CreateAccountButton(QWidget* parent, QtServiceModel* model);
  virtual ContactWidget* CreateContactWidget(QWidget* parent, Contact::Ref contact);
  //virtual PostWidget* CreatePostWidget()=0;
  virtual std::vector<WidgetFactory*> CreateWidgetFactories()=0;
};

} /* namespace sdc */

#endif /* QT_SERVICE_H_ */
