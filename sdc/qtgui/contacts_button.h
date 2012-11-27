/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CONTACTS_BUTTON_H_
#define CONTACTS_BUTTON_H_

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

} // namespace sdc

#endif // CONTACTS_BUTTON_H_
