#include "core.h"
#include "new_contact_dialog.h"
#include "account.h"
#include "service.h"
#include "qt_service.h"
#include "service_model.h"
#include "new_contact_widget.h"
#include <QtGui/QMessageBox>
#include "boost/cast.hpp"

namespace sdc {

NewContactDialog::NewContactDialog(QWidget *parent)
    : QDialog(parent) {
	ui.setupUi(this);


	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(ui.servicesComboBox, SIGNAL(currentIndexChanged(int)),
	    this, SLOT(ChangeServicePane(int)));

  int i = 1;
  for (const ServiceModel* model : Core::Instance()->models()) {
    QString id = QString::fromStdString(model->account()->GetId());
    QVariant data(id);
    ui.servicesComboBox->insertItem(i, QString::fromStdString(model->account()->GetUid()), data);
    ++i;
  }

	ui.servicesComboBox->insertSeparator(1);

}

NewContactDialog::~NewContactDialog() {}

void NewContactDialog::ChangeServicePane(int index) {

  if (ui.currentServiceWidget) {
    layout()->removeWidget(ui.currentServiceWidget);
    ui.currentServiceWidget->setAttribute(Qt::WA_DeleteOnClose);
    ui.currentServiceWidget->close();
    ui.currentServiceWidget = 0;
  }

  if (index > 1) {
    std::string account_id = ui.servicesComboBox->itemData(index).toString().toStdString();
    Service* s = Core::Instance()->model(account_id)->service();
    QtService* qs = boost::polymorphic_downcast<QtService*>(s);
    contact_pane_ = qs->CreateNewContactWidget();
    boost::polymorphic_downcast<QVBoxLayout*>(layout())->insertWidget(1, contact_pane_);
    ui.currentServiceWidget = contact_pane_;
  }
}

void NewContactDialog::accept() {
  if (contact_pane_->IsInputValid()) {
    std::string id = ui.servicesComboBox->itemData(ui.servicesComboBox->currentIndex()).toString().toStdString();
    contact_pane_->Process(Core::Instance()->model(id));
    this->close();
  } else {
    QMessageBox::warning(this, "Contact error", contact_pane_->GetErrorMessage());
  }
}

} /* namespace sdc */
