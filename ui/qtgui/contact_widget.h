#ifndef CONTACT_WIDGET_H
#define CONTACT_WIDGET_H

#include "content.h"
#include "qt_view.h"
#include <QtGui/QWidget>
#include "ui_contact_widget.h"

namespace sdc {

class ContactWidget : public QWidget, public QtView {

    Q_OBJECT

 public:
  ContactWidget(QWidget *parent, Contact::Ref contact);
  ~ContactWidget();

 signals:
  void remove();

 private slots:
  void Remove() {
    this->close();
  }

 private:
  void OnRemove() {
    emit remove();
  }

  Contact::Ref model_;
  Ui::ContactWidgetClass ui;
  MainWindow* parent_;
};

} /* namespace sdc */

#endif // CONTACT_WIDGET_H
