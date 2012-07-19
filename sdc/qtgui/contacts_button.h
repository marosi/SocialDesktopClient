#ifndef CONTACTS_BUTTON_H
#define CONTACTS_BUTTON_H

#include "prime_button.h"

namespace sdc {

class ContactsPanel;

class ContactsButton : public PrimeButton {

    Q_OBJECT

 public:
  ContactsButton();

  void SetPanel(ContactsPanel* panel);

 private slots:
  void UncheckButton();

 private:
  ContactsPanel* contacts_;
};

} /* namespace sdc */

#endif // CONTACTS_BUTTON_H
