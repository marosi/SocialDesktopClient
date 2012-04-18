#include "new_account_dialog.h"
#include "log.h"
#include "new_account_widget.h"
#include "service.h"
#include "qt_service.h"
#include "config_manager.h"
#include "account_data.h"
#include <QVariant>
#include <QMessageBox>
#include "boost/foreach.hpp"
#include "boost/cast.hpp"

namespace sdc {

NewAccountDialog::NewAccountDialog(QWidget *parent)
    : QDialog(parent), QtView(parent), action_(NEW), account_(0)
{
	ui.setupUi(this);
	this->setAttribute(Qt::WA_DeleteOnClose);

	int i = 0;
  BOOST_FOREACH (Service* s, core()->services()) {
    QString signature = QString::fromStdString(s->GetSignature());
    QVariant data(signature);
    ui.servicesComboBox->insertItem(i, QString::fromStdString(s->name()), data);
    QtService* qs = boost::polymorphic_downcast<QtService*>(s);
    NewAccountWidget* pane = qs->CreateNewAccountPane();
    pane->setParent(this);
    pane->hide();
    service_panes_[signature] = pane;
    combobox_service_index_[signature] = i;
    ++i;
  }

	// interactions
	connect(ui.servicesComboBox, SIGNAL(activated(int)),
	    this, SLOT(ChangeServicePane(int)));
	connect(ui.servicesComboBox, SIGNAL(currentIndexChanged(int)),
	    this, SLOT(ChangeServicePane(int)));
	connect(ui.servicesComboBox, SIGNAL(highlighted(int)),
	      this, SLOT(ChangeServicePane(int)));
	// finally
	this->show();
}

NewAccountDialog::~NewAccountDialog() {}

void NewAccountDialog::SetAccountForEditing(AccountData* account) {
  QString signature = QString::fromStdString(account->GetServiceSignature());
  ChangeServicePane(combobox_service_index_[signature]);
  service_panes_[signature]->BindDataFrom(account);
  account_ = account;
  action_ = EDIT;
}

void NewAccountDialog::accept() {
  if(current_pane_->IsDataValid()) {
    if (account_) {
      current_pane_->BindDataTo(account_);
    } else { // Create new account
      AccountData* account = current_pane_->CreateNew();
      QVariant data = ui.servicesComboBox->itemData(ui.servicesComboBox->currentIndex());
      account->SetServiceSignature(data.toString().toStdString());
      core()->data()->AddAccount(account);
    }
    this->close();
  } else {
    QMessageBox::warning(this, "Account data error", current_pane_->GetErrorMessage());
  }
}

void NewAccountDialog::ChangeServicePane(int index) {
  LOG(DEBUG2) << "Changing service pane";
  QVariant data = ui.servicesComboBox->itemData(index);
  //int at = this->layout()->indexOf(ui.currentServiceWidget);
  ui.layout->removeWidget(current_pane_);  // TODO: Test when second plugin will be available
  current_pane_ = service_panes_[data.toString()];
  ui.layout->insertWidget(2, current_pane_);
  current_pane_->show();
}

} /* namespace sdc */
