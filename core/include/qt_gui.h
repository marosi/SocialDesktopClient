/**
 * @file qt_gui.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 31, 2012
 */

#ifndef QT_GUI_H_
#define QT_GUI_H_

#include "ui.h"
#include <QApplication>

namespace sdc {

class QtSettingsController;
class MainWindow;

class QtGui : public UI {
 public:
  QtGui(Core* core, int argc, char* argv[])
      : UI(core),
        app_(argc, argv),
        main_view_(0),
        settings_controller_(0) {}

  void Init();

  int Exec() {
    return app_.exec();
  }

 private:
  QApplication app_;
  MainWindow* main_view_;
  /*
   * Controllers
   */
  QtSettingsController* settings_controller_;
};

} /* namespace sdc */

#endif /* QT_GUI_H_ */
