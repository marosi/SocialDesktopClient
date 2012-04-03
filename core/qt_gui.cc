/*
 * qt_gui.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Apr 2, 2012
 */

#include "qt_gui.h"
#include "mainwindow.h"
#include "connection_manager.h"
#include "qt_service_controller.h"
#include "qt_settings_controller.h"
#include "boost/foreach.hpp"
#include "boost/cast.hpp"
#include <vector>

namespace sdc {

void QtGui::Init() {
    // Init core models, controllers and views
    UI::Init();

    //TestControllerInit();
    main_view_ = new MainWindow;
    BOOST_FOREACH (const ServiceController* sc, GetControllers()) {
      const QtServiceController* qsc = boost::polymorphic_downcast<const QtServiceController*>(sc);
      main_view_->AddServiceWidget(qsc->GetView());
    }

    settings_controller_ = CreateController<QtSettingsController>();


    main_view_->SetSettingsView(settings_controller_->GetView());

    // show main window finally
    main_view_->show();
  }

} /* namespace sdc */
