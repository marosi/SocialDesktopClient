/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "contacts_button.h"
#include "contacts_panel.h"

namespace sdc {

ContactsButton::ContactsButton() {
  button()->setText("Contacts");
  button()->setObjectName("ContactsButton");
  button()->setCheckable(true);
  button()->setChecked(true);
  // set icon
  button()->setIcon(QIcon(":/icons/Address Book.ico"));
  button()->setIconSize(QSize(30, 30));
}

void ContactsButton::SetPanel(ContactsPanel* panel) {
  contacts_ = panel;
  connect(button(), SIGNAL(toggled(bool)), contacts_, SLOT(setVisible(bool)));
  connect(contacts_, SIGNAL(closed()), this, SLOT(UncheckButton()));
}

void ContactsButton::UncheckButton() {
  button()->setChecked(false);
}

} // namespace sdc
