#include "account_button.h"
#include "qt_service_model.h"
#include "account.h"
#include "log.h"
#include <QMenu>
#include <QDataWidgetMapper>
#include "boost/cast.hpp"

namespace sdc {

AccountButton::AccountButton(QtServiceModel* model)
    : model_(model) {
  // set menu
  menu_ = new QMenu(this);
  QString account_name = QString::fromStdString(model_->account()->GetUid());
  title_ = menu_->addAction(account_name);
  menu_->addSeparator();
  // online/offline actions
  connect(menu_->addAction("Online"), SIGNAL(triggered()),
      this, SLOT(GoOnline()));
  connect(menu_->addAction("Offline"), SIGNAL(triggered()),
      this, SLOT(GoOffline()));
  // ...and install in button
  button()->setMenu(menu_);
  button()->setPopupMode(QToolButton::InstantPopup);
}

AccountButton::~AccountButton() {}

void AccountButton::SetAccountIcon(const QIcon &icon) {
  title_->setIcon(icon);
}

void AccountButton::GoOnline() {
  model_->Connect();
}

void AccountButton::GoOffline() {
  model_->Disconnect();
}

} /* namespace sdc */
