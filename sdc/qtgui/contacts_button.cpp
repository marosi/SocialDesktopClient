#include "contacts_button.h"
#include "contacts_panel.h"

namespace sdc {

ContactsButton::ContactsButton() {
  button()->setText("C");
  button()->setCheckable(true);
  button()->setChecked(true);
}

void ContactsButton::SetPanel(ContactsPanel* panel) {
  contacts_ = panel;
  connect(button(), SIGNAL(toggled(bool)), contacts_, SLOT(setVisible(bool)));
  connect(contacts_, SIGNAL(closed()), this, SLOT(UncheckButton()));
}

void ContactsButton::UncheckButton() {
  button()->setChecked(false);
}

} /* namespace sdc */
