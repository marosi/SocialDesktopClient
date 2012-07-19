#include "contacts_panel.h"
#include "new_contact_dialog.h"
#include <QToolButton>

namespace sdc {

ContactsPanel::ContactsPanel() {
  title_label()->setText("Contacts");
  add_button_ = new QToolButton(this);
  AddTitleBarButton(add_button_);
  connect(add_button_, SIGNAL(clicked()), this, SLOT(ShowNewContactDialog()));
}

void ContactsPanel::ShowNewContactDialog() {
  NewContactDialog* dialog = new NewContactDialog(this);
  dialog->exec();
}

} /* namespace sdc */
