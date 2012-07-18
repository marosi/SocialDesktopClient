#ifndef CONTACT_WIDGET_H
#define CONTACT_WIDGET_H

#include "content.h"
#include "qt_view.h"
#include <QtGui/QWidget>
#include <QToolButton>
#include "ui_contact_widget.h"

namespace sdc {

class ContactWidget : public QWidget {

    Q_OBJECT

 public:
  ContactWidget(Contact* contact);
  virtual ~ContactWidget();

  QLabel* avatar_label() {
    return ui.avatarLabel;
  }

  Contact* model() {
    return model_;
  }

 protected slots:
  virtual void Remove() {}

 private:
  Contact* model_;
  Ui::ContactWidgetClass ui;
  QToolButton* close_button_;
};

} /* namespace sdc */

#endif // CONTACT_WIDGET_H
