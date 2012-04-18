/**
 * @file settings_dialog_controller.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 13, 2012
 */

#ifndef SETTINGS_DIALOG_CONTROLLER_H_
#define SETTINGS_DIALOG_CONTROLLER_H_

#include "settings_dialog.h"
#include "qt_controller.h"

namespace sdc {

class SettingsDialogController : public QtController {

  Q_OBJECT

 public:
  SettingsDialogController(SettingsDialog* view) : QtController(view) {
    view_ = view;
    view_->show();
  }

 private:
  SettingsDialog* view_;
};

} /* namespace sdc */

#endif /* SETTINGS_DIALOG_CONTROLLER_H_ */
