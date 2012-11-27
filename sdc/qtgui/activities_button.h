/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef ACTIVITIES_BUTTON_H_
#define ACTIVITIES_BUTTON_H_

#include "prime_button.h"

namespace sdc {

class ActivitiesPanel;

class ActivitiesButton : public PrimeButton {

    Q_OBJECT

 public:
  ActivitiesButton();

  void SetPanel(ActivitiesPanel* panel);

 private slots:
  void UncheckButton();

 private:
  ActivitiesPanel* activities_;
};

} // namespace sdc

#endif // ACTIVITIES_BUTTON_H_
