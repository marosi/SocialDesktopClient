/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef ACTIVITIES_PANEL_H_
#define ACTIVITIES_PANEL_H_

#include "content_panel.h"
#include <QList>

namespace sdc {

class Activity;

/**
 * Specific content panel for activities.
 */
class ActivitiesPanel : public ContentPanel {
 public:
  ActivitiesPanel();

  /**
   * Adds activity widget to the top of content area.
   * @param activity
   */
  void AddActivity(Activity* activity);

 private:
  QList<Activity*> activities_;
};

} // namespace sdc

#endif // ACTIVITY_PANEL_H_
