#include "activities_panel.h"
#include "activity.h"

namespace sdc {

ActivitiesPanel::ActivitiesPanel() {
  title_label()->setText("Activities");
}

void ActivitiesPanel::AddActivity(Activity* activity) {
  content_layout()->insertWidget(0, activity);
}

} /* namespace sdc */
