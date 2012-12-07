/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef BC_NEW_CONTACT_WIDGET_H_
#define BC_NEW_CONTACT_WIDGET_H_

#include "bc_contact.h"
#include "bc_model.h"
#include "sdc/qtgui/new_contact_widget.h"
#include <QtGui/QWidget>
#include "ui_bc_new_contact_widget.h"
#include "boost/cast.hpp"

/**
 * buddycloud new contact widget implementation.
 */
class BcNewContactWidget : public sdc::NewContactWidget {

    Q_OBJECT

 public:
  BcNewContactWidget(QWidget */*parent*/ = 0) {
    ui.setupUi(this);
  }
  ~BcNewContactWidget() {}

  bool IsInputValid() {
    return true;
  }

  void Process(sdc::ServiceModel* model) {
    BcModel* m = boost::polymorphic_downcast<BcModel*>(model);
    m->SubscribeToChannel(ui.lineEdit->text().toStdString());
  }

 private:
  Ui::BcNewContactWidgetClass ui;
};

#endif // BC_NEW_CONTACT_WIDGET_H_
