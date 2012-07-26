#ifndef MAIN_BUTTON_H
#define MAIN_BUTTON_H

#include "prime_button.h"

namespace sdc {

class MainWindow;
class SettingsDialog;

class MainButton : public PrimeButton {

    Q_OBJECT

 public:
  MainButton(MainWindow* main_window_);

 private slots:
  void ShowSettingsDialog();

 private:
  MainWindow* main_window_;
  SettingsDialog* settings_;
};

} /* namepsace sdc */

#endif // MAIN_BUTTON_H
