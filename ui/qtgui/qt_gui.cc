/*
 * qt_gui.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Apr 2, 2012
 */

#include "qt_gui.h"
#include "mainwindow.h"
#include "connection_manager.h"
#include "config_manager.h"
#include "qt_service_model.h"

#include "boost/foreach.hpp"
#include "boost/bind.hpp"
#include "boost/cast.hpp"
#include <vector>

using namespace boost;

namespace sdc {

void QtGui::Init() {
  // Init core models, controllers and views
  UI::Init();

  /*
   * Register core class for Qt meta processing
   * NOTE: For the type to be handled by QVariant it needs to be
   * added by macro Q_DECLARE_METATYPE(TypeName)
   */
  //qRegisterMetaType)

  /*
   * Connect core signals
   */
  core()->data()->onAccountsChanged.connect(bind(&QtGui::OnAccountsChanged, this));
  core()->onGuiPrepared.connect(bind(&QtGui::OnGuiPrepared, this));
  core()->onAccountActivated.connect(bind(&QtGui::OnAccountActivated, this, _1));
  core()->onAccountDeactivated.connect(bind(&QtGui::OnAccountDeactivated, this, _1));

  /*
   * Main window initialization
   */
  main_view_ = new MainWindow(this);
  /*BOOST_FOREACH (const ServiceController* sc, GetControllers()) {
    const QtServiceController* qsc = boost::polymorphic_downcast<const QtServiceController*>(sc);
    main_view_->AddServiceWidget(qsc->GetView());
  }*/ // TODO: REMOVE there is no controllers

  /*
   * signal/slot connections
   */
  connect(this, SIGNAL(accountActivated(QtServiceModel*)),
      main_view_, SLOT(ActivateAccount(QtServiceModel*)));
  connect(this, SIGNAL(accountDeactivated(QtServiceModel*)),
        main_view_, SLOT(DeactivateAccount(QtServiceModel*)));

  BOOST_FOREACH (ServiceModel* model, core()->GetModels()) {
    QtServiceModel* qt_model = dynamic_cast<QtServiceModel*>(model);
    emit accountActivated(qt_model);
  }

  // show main window finally
  main_view_->show();
}


void QtGui::OnAccountActivated(AccountData* account) {
  LOG(DEBUG)<< "qtgui : account actiovated";
  QtServiceModel* qt_model = dynamic_cast<QtServiceModel*>(account->GetServiceModel());
  if (qt_model)
    emit accountActivated(qt_model);
}
void QtGui::OnAccountDeactivated(AccountData* account) {
  QtServiceModel* qt_model = dynamic_cast<QtServiceModel*>(account->GetServiceModel());
  if (qt_model)
    emit accountDeactivated(qt_model);
}

} /* namespace sdc */
