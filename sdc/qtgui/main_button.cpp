#include "main_button.h"
#include "main_window.h"
#include "settings_dialog.h"
#include <QMenu>

namespace sdc {

MainButton::MainButton(MainWindow* main_window)
  : main_window_(main_window), settings_(0) {

  button()->setText("Global");
  // set icon
  button()->setIcon(QIcon(":/icons/Setting.ico"));
  button()->setIconSize(QSize(30, 30));
  button()->setStyleSheet(
    "QToolButton::menu-indicator { image: none }"
  );

  button()->setMenu(new QMenu);
  QAction* settings = button()->menu()->addAction("Settings");
  button()->menu()->addSeparator();
  QAction* quit = button()->menu()->addAction("Quit");
  button()->setPopupMode(QToolButton::DelayedPopup);

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


