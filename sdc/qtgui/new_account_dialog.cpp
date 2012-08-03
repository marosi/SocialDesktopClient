#include "account.h"
#include "core.h"
#include "data_manager.h"
#include "log.h"
#include "new_account_dialog.h"
#include "new_account_widget.h"
#include "service.h"
#include "qt_service.h"
#include <QVariant>
#include <QMessageBox>
#include "boost/cast.hpp"
#include "boost/foreach.hpp"

namespace sdc {

NewAccountDialog::NewAccountDialog(QWidget *parent)
    : QDialog(parent), action_(NEW), account_(0) {

	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);

	// interactions
	connect(ui.servicesComboBox, SIGNAL(currentIndexChanged(int)),
	      this, SLOT(ChangeServicePane(int)));

	ui.servicesComboBox->insertSeparator(1);
	// load available services
	int i = 2;
  BOOST_FOREACH (Service* s , Core::Instance()->services()) {
    QString signature = QString::fromStdString(s->signature());
    QVariant data(signature);
    ui.servicesComboBox->insertItem(i, QString::fromStdString(s->name()), data);
    QtService* qs = boost::polymorphic_downcast<QtService*>(s);
    NewAccountWidget* pane = qs->CreateNewAccountPane();
    pane->setParent(this);
    pane->hide();
    ui.layout->insertWidget(i, pane);
    service_panes_[signature] = pane;
    combobox_service_index_[signature] = i;
    ++i;
  }
  current_pane_ = *service_panes_.begin();



  show();
}

NewAccountDialog::~NewAccountDialog() {}

void NewAccountDialog::SetAccountForEditing(Account* account) {
  QString signature = QString::fromStdString(account->GetServiceSignature());
  ui.servicesComboBox->setCurrentIndex(combobox_service_index_[signature]);
  service_panes_[signature]->BindDataFrom(account);
  account_ = account;
  action_ = EDIT;
}

void NewAccountDialog::accept() {
  if(current_pane_->IsDataValid()) {
    if (account_) {
      current_pane_->BindDataTo(account_);
    } else { // Create new account
      Account* account = current_pane_->CreateNew();
      QVariant data = ui.servicesComboBox->itemData(ui.servicesComboBox->currentIndex());
      account->SetServiceSignature(data.toString().toStdString());
      Core::Instance()->data()->AddAccount(account);
    }
    this->close();
  } else {
    QMessageBox::warning(this, "Account data error", current_pane_->GetErrorMessage());
  }
}

void NewAccountDialog::ChangeServicePane(int index) {
  LOG(DEBUG2) << "Changing service pane";
  //int at = this->layout()->indexOf(ui.currentServiceWidget);
  //ui.layout->removeWidget(current_pane_);
  current_pane_->hide();
  if (index > 1) {
    QVariant data = ui.servicesComboBox->itemData(index);
    current_pane_ = service_panes_[data.toString()];
    current_pane_->show();
  }
}

} /* namespace sdc */
