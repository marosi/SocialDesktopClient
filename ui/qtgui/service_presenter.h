/**
 * @file service_presenter.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jul 5, 2012
 */

#ifndef SERVICE_PRESENTER_H_
#define SERVICE_PRESENTER_H_

#include "account_button.h"
#include "bind.h"
#include "main_window.h"
#include "qt_service_model.h"
#include <QObject>

namespace sdc {

class ServicePresenter : public QObject {

    Q_OBJECT

 public:
  friend class QtGui;
  friend class MainWindow;

  virtual ~ServicePresenter() {}
  virtual void Init() {}

  MainWindow* main_window() {
    return main_window_;
  }

  QtServiceModel* model() {
    return service_model_;
  }

  template<class T>
  T* model() {
      return boost::polymorphic_downcast<T*>(service_model_);
  }

  AccountButton* account_button() {
    return account_button_;
  }

 private:
  MainWindow* main_window_;
  QtServiceModel* service_model_;
  AccountButton* account_button_;
};

} /* namespace sdc */

#endif /* SERVICE_PRESENTER_H_ */
