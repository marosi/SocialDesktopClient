#ifndef CONTACT_WIDGET_H
#define CONTACT_WIDGET_H

#include "qt_view.h"
#include <QtGui/QWidget>
#include <QToolButton>
#include "ui_contact_widget.h"

namespace sdc {

class Contact;

class ContactWidget : public QWidget {

    Q_OBJECT

 public:
  ContactWidget(Contact* contact);
  virtual ~ContactWidget();

  QLabel* avatar_label() { return ui.avatarLabel; }
  Contact* model() { return contact_; }

 private:
  Contact* contact_;
  Ui::ContactWidgetClass ui;
  QToolButton* close_button_;
};

} /* namespace sdc */

#endif // CONTACT_WIDGET_H
