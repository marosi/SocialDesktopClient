#include "activity_panel.h"
#include "activity.h"

namespace sdc {

ActivityPanel::ActivityPanel() {
  title_label()->setText("Activities");
}

void ActivityPanel::AddActivity(Activity* activity) {
  content_layout()->insertWidget(0, activity);
}

} /* namespace sdc */
