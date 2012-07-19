#ifndef CONTACTS_PANEL_H
#define CONTACTS_PANEL_H

#include "content_panel.h"

class QToolButton;

namespace sdc {

class ContactsPanel : public ContentPanel {

    Q_OBJECT

 public:
  ContactsPanel();

 public slots:
  void ShowNewContactDialog();

 private:
  QToolButton* add_button_;
};

} /* namespace sdc */

#endif // CONTACTS_PANEL_H
