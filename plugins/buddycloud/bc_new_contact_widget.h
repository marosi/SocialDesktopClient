#ifndef BC_NEW_CONTACT_WIDGET_H
#define BC_NEW_CONTACT_WIDGET_H

#include "sdc.h"
#include "bc_contact.h"
#include "bc_model.h"
#include <QtGui/QWidget>
#include "ui_bc_new_contact_widget.h"
#include "boost/cast.hpp"

class BcNewContactWidget : public sdc::NewContactWidget {

    Q_OBJECT

 public:
  BcNewContactWidget(QWidget *parent = 0) {
    ui.setupUi(this);
  }
  ~BcNewContactWidget() {}

  bool IsInputValid() {
    return true;
  }

  void Process(sdc::ServiceModel* model) {
    /*BcModel* bc = boost::polymorphic_downcast<BcModel*>(model);
    BcContact::Ref contact(new BcContact);
    contact->SetUid(ui.lineEdit->text().toStdString());
    LOG(DEBUG) << "Contact " << contact->GetUid() << " are about to be process by buddycloud model";
    bc->AddNewContact(contact);*/
  }

 private:
  Ui::BcNewContactWidgetClass ui;
};

#endif // BC_NEW_CONTACT_WIDGET_H
