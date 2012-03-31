/*
 * ui.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Mar 31, 2012
 */

#include "ui.h"
#include "connection.h"
#include "connection_manager.h"
#include "core.h"
#include "service_controller.h"
#include "service_model.h"
#include "boost/shared_ptr.hpp"
#include "boost/foreach.hpp"

namespace sdc {

Ui::~Ui() {
  BOOST_FOREACH (ServiceController* ctrl, controllers_) {
    delete ctrl;
  }
}

void Ui::Init() {
  // Launch GUI of each initiated service connection
  std::vector<ConnectionRef> conns;
  core()->connection_manager()->GetAllActiveConnections(conns); // TOOD: connection_manager is more of service_manager...
  std::vector<ConnectionRef>::iterator it;
  for(it = conns.begin(); it != conns.end(); ++it) {
    ConnectionRef conn = (*it);
    //conn->service()->InitializeGui(main_view_);
    // create service controller for connection
    ServiceController* service_controller = CreateServiceController(conn->service());
    boost::shared_ptr<ServiceController> tmp(service_controller);
    conn->SetController(tmp);
    service_controller->SetConnection(conn);
    // set controller's view and model
    //ServiceViewRef sview = conn->service()->CreateMainView();
    ServiceModelRef smodel = conn->service()->CreateMainModel();
    service_controller->SetModel(smodel);
    //sctrler->SetView(sview);
    //sview->SetController(sctrler);
    //sview->SetModel(smodel);
    //main_view_->StartUpServiceMainWidget(boost::dynamic_pointer_cast<QWidget>(view));
    service_controller->Initiate();
  }
}

ServiceController* Ui::CreateServiceController(Service* service) {
  LOG(DEBUG) << "Ui::CreateServiceController call";
  ServiceController* result = service->CreateServiceController();
  controllers_.push_back(result);
  return result;
}

} /* namespace sdc */
