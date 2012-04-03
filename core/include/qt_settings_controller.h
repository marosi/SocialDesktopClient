/**
 * @file qt_settings_controller.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 31, 2012
 */

#ifndef QT_SETTINGS_CONTROLLER_H_
#define QT_SETTINGS_CONTROLLER_H_

#include "controller.h"
#include "settings_dialog.h"

namespace sdc {

class QtSettingsController : public QtController {
 public:
  QtSettingsController() : view_(new SettingsDialog) {
    SetControllersMainView(view_);
  }

 private:
  SettingsDialog* view_;
};

} /* namespace sdc */

#endif /* QT_SETTINGS_CONTROLLER_H_ */
