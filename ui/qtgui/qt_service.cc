/*
 * qt_service.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Apr 20, 2012
 */

#include "qt_service.h"
#include "account_button.h"

#include "log.h"

namespace sdc {

AccountButton* QtService::CreateAccountButton(QWidget* parent, QtServiceModel* model) {
  return new AccountButton(parent, model);
}

} /* namespace sdc */
