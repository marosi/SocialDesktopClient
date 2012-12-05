/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef SERVICE_PRESENTER_H_
#define SERVICE_PRESENTER_H_

#include "account_button.h"
#include "bind.h"
#include "contacts_panel.h"
#include "main_window.h"
#include "qt_service_model.h"
#include <QObject>
#include "boost/cast.hpp"

namespace sdc {

class ServicePresenter : public QObject {

    Q_OBJECT

 public:
  friend class QtGui;
  friend class MainWindow;

  ServicePresenter();
  virtual ~ServicePresenter() {}
  virtual void Init() {}

  MainWindow* main_window() const {
    return main_window_;
  }

  QtServiceModel* model() const {
    return service_model_;
  }

  template<class T>
  T* model() const {
    return boost::polymorphic_downcast<T*>(service_model_);
  }

  AccountButton* account_button() const {
    return account_button_;
  }

  void SetStylesheet(const QString &stylesheet);

 private:
  QtGui* qtgui_;
  MainWindow* main_window_;
  QtServiceModel* service_model_;
  AccountButton* account_button_;
};

} // namespace sdc

#endif // SERVICE_PRESENTER_H_
