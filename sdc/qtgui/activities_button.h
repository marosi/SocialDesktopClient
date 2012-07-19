#ifndef ACTIVITIES_BUTTON_H
#define ACTIVITIES_BUTTON_H

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

} /* namespace sdc */

#endif // ACTIVITIES_BUTTON_H
