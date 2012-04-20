#include "account_button.h"
#include "qt_service_model.h"
#include "account_data.h"
#include "log.h"
#include <QMenu>

namespace sdc {

AccountButton::AccountButton(QWidget* parent, QtServiceModel* model)
    : QWidget(parent), QtView(parent), model_(model) {
	ui.setupUi(this);
	ui.toolButton->setText(QString::fromStdString(model->GetAccount()->GetUid()));

	menu_ = new QMenu(QString::fromStdString(model_->GetAccount()->GetUid()), this);
	connect(menu_->addAction("Online"), SIGNAL(triggered()),
	    this, SLOT(GoOnline()));
	connect(menu_->addAction("Offline"), SIGNAL(triggered()),
	    this, SLOT(GoOffline()));

	connect(ui.toolButton, SIGNAL(clicked()),
	    this, SLOT(ShowMenu()));

}

AccountButton::~AccountButton() {}

void AccountButton::ShowMenu() {
  QAction* action = menu_->exec(QCursor::pos());
}

void AccountButton::GoOnline() {
  LOG(DEBUG) << "CUUUUZ";
  model_->Connect();
}

void AccountButton::GoOffline() {
  model_->Disconnect();
}

} /* namespace sdc */
