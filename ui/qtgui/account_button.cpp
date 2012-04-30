#include "account_button.h"
#include "qt_service_model.h"
#include "account_data.h"
#include "log.h"
#include <QMenu>

namespace sdc {

AccountButton::AccountButton(QWidget* parent, QtServiceModel* model)
    : QWidget(parent), QtView(parent), model_(model) {
	ui.setupUi(this);

	QString account_name = QString::fromStdString(model_->GetAccount()->GetUid());

	QColor color;
	color.setNamedColor("sienna");
	QPalette palette;
	palette.setColor(QPalette::Button, color);
	ui.toolButton->setPalette(palette);
	ui.toolButton->setToolTip(account_name);

	menu_ = new QMenu(account_name, this);
	QAction* title = menu_->addAction(QString::fromStdString(model->GetAccount()->GetUid()));
	title->setEnabled(false);
	menu_->addSeparator();
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
  model_->Connect();
}

void AccountButton::GoOffline() {
  model_->Disconnect();
}

} /* namespace sdc */
