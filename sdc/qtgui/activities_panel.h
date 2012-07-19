#ifndef ACTIVITIES_PANEL_H
#define ACTIVITIES_PANEL_H

#include "content_panel.h"
#include <QList>

namespace sdc {

class Activity;

class ActivitiesPanel : public ContentPanel {
 public:
  ActivitiesPanel();

  void AddActivity(Activity* activity);

 private:
  QList<Activity*> activities_;
};

} /* namespace sdc */

#endif // ACTIVITY_PANEL_H
