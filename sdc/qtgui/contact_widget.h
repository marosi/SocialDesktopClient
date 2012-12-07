/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CONTACT_WIDGET_H_
#define CONTACT_WIDGET_H_

#include <QtGui/QWidget>
#include <QToolButton>
#include "ui_contact_widget.h"

namespace sdc {

class Contact;

/**
 * Common widget for all application contacts.
 */
class ContactWidget : public QWidget {

    Q_OBJECT

 public:
  /**
   * Contstructs contact widget for particular contact.
   * @param contact
   */
  ContactWidget(Contact* contact);
  virtual ~ContactWidget();

  /**
   * Gets image label for avatar.
   * @return label
   */
  QLabel* avatar_label() { return ui.avatarLabel; }
  /**
   * Gets contact model.
   * @return
   */
  Contact* model() { return contact_; }

 private:
  Contact* contact_;
  Ui::ContactWidgetClass ui;
  QToolButton* close_button_;
};

} // namespace sdc

#endif // CONTACT_WIDGET_H_
