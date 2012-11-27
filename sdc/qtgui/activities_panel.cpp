/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "activities_panel.h"
#include "activity.h"

namespace sdc {

ActivitiesPanel::ActivitiesPanel() {
  title_label()->setText("Activities");
  setObjectName("ActivitiesPanel");
}

void ActivitiesPanel::AddActivity(Activity* activity) {
  content_layout()->insertWidget(0, activity);
}

} // namespace sdc
