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
#include "qt_service_controller.h"
#include "boost/foreach.hpp"
#include "boost/bind.hpp"
#include "boost/cast.hpp"
#include <vector>

using namespace boost;

namespace sdc {

void QtGui::Init() {
  // Init core models, controllers and views
  UI::Init();

  this->ConnectCoreSignals();

  main_view_ = new MainWindow(this);
  BOOST_FOREACH (const ServiceController* sc, GetControllers()) {
    const QtServiceController* qsc = boost::polymorphic_downcast<const QtServiceController*>(sc);
    main_view_->AddServiceWidget(qsc->GetView());
  }
  // show main window finally
  main_view_->show();
}

void QtGui::ConnectCoreSignals() {
  core()->data()->onAccountsChanged.connect(bind(&QtGui::OnAccountsChanged, this));
  core()->onGuiPrepared.connect(bind(&QtGui::OnGuiPrepared, this));
}

} /* namespace sdc */
