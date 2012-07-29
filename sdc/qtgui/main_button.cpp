#include "main_button.h"
#include "main_window.h"
#include "settings_dialog.h"
#include <QMenu>

namespace sdc {

MainButton::MainButton(MainWindow* main_window)
  : main_window_(main_window), settings_(0) {
  button()->setText("Global");
  button()->setObjectName("MainButton");
  // set icon
  button()->setIcon(QIcon(":/icons/Setting.ico"));
  button()->setIconSize(QSize(30, 30));
  // set menu
  QMenu* menu = new QMenu;
  QAction* settings = menu->addAction("Settings");
  menu->addSeparator();
  QAction* quit = menu->addAction("Quit");
  button()->setMenu(menu);
  button()->setPopupMode(QToolButton::DelayedPopup);
  // bind signal and slots
  connect(quit, SIGNAL(triggered()), main_window_, SLOT(close()));
  connect(settings, SIGNAL(triggered()), this, SLOT(ShowSettingsDialog()));
  connect(button(), SIGNAL(clicked()), this, SLOT(ShowSettingsDialog()));
}

void MainButton::ShowSettingsDialog() {
  if (!settings_)
    settings_ = new SettingsDialog(this);
  else
    settings_->show();
}

} /* namespace sdc */


