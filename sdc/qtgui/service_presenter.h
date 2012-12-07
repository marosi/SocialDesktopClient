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

/**
 * The service presenter class.
 * Encapsulates all Qt GUI presentation bussiness logic within service implementation.
 */
class ServicePresenter : public QObject {

    Q_OBJECT

 public:
  friend class QtGui;
  friend class MainWindow;

  ServicePresenter();
  virtual ~ServicePresenter() {}
  /**
   * Initializes service presenter.
   */
  virtual void Init() {}

  /**
   * Gets main application window.
   * @return main window
   */
  MainWindow* main_window() const {
    return main_window_;
  }

  /**
   * Gets associated service model
   * @return associated service model
   */
  QtServiceModel* model() const {
    return service_model_;
  }

  /**
   * Get associated service model downcasted as required.
   * @return associated service model instance of required type
   */
  template<class T>
  T* model() const {
    return boost::polymorphic_downcast<T*>(service_model_);
  }

  /**
   * Gets main account prime button.
   * @return account prime button
   */
  AccountButton* account_button() const {
    return account_button_;
  }

  /**
   * Sets Qt stylesheet for styling service components.
   * @param stylesheet
   */
  void SetStylesheet(const QString &stylesheet);

 private:
  QtGui* qtgui_;
  MainWindow* main_window_;
  QtServiceModel* service_model_;
  AccountButton* account_button_;
};

} // namespace sdc

#endif // SERVICE_PRESENTER_H_
