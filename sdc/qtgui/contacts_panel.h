/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CONTACTS_PANEL_H_
#define CONTACTS_PANEL_H_

#include "content_panel.h"
#include <QStackedWidget>

class QToolButton;

namespace sdc {

class ContactsPanel : public ContentPanel {

    Q_OBJECT

 public:
  ContactsPanel();

 public slots:
  /**
   * Shows dialog for adding the contact globally.
   */
  void ShowNewContactDialog();

  QStackedWidget* stacked_widget() {
    return stack_;
  }

 private:
  QToolButton* add_button_;
  QStackedWidget* stack_;
};

} // namespace sdc

#endif // CONTACTS_PANEL_H_
