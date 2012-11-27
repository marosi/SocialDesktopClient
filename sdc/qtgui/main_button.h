/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef MAIN_BUTTON_H_
#define MAIN_BUTTON_H_

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

} // namespace sdc

#endif // MAIN_BUTTON_H_
