#ifndef ACTIVITY_PANEL_H
#define ACTIVITY_PANEL_H

#include "content_panel.h"
#include <QList>

namespace sdc {

class Activity;

class ActivityPanel : public ContentPanel {
 public:
  ActivityPanel();

  void AddActivity(Activity* activity);

 private:
  QList<Activity*> activities_;
};

} /* namespace sdc */

#endif // ACTIVITY_PANEL_H
