/**
 * @file qt_service_controller.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 31, 2012
 */

#ifndef QT_SERVICE_CONTROLLER_H_
#define QT_SERVICE_CONTROLLER_H_

#include "service_controller.h"
#include <QtGui/QWidget>

namespace sdc {

class QtServiceController : public ServiceController {
 public:
  QWidget* GetView() const {
    return view_;
  }

 protected:
  /**
   * In order to add service view to application layout
   * child classes MUST set their QWidget
   */
  void SetControllersMainView(QWidget* view) {
    view_ = view;
  }

 private:
  QWidget* view_;
};

} /* namespace sdc */

#endif /* QT_SERVICE_CONTROLLER_H_ */
