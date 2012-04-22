/*
 * buddycloud_service.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Feb 7, 2012
 */

#include "buddycloud_service.h"
#include "b_controller.h"
#include "b_model.h"
#include "b_view.h"
#include "buddycloud_connection.h"
#include "buddycloud_model.h"
#include "bc_new_account_widget.h"

using namespace sdc;

/*void BuddycloudService::InitializeGui(MainView* main_view) {
  boost::shared_ptr<BModel> m(new BModel);
  boost::shared_ptr<BView> v(new BView);
  boost::shared_ptr<BController> c(new BController);

  c->SetModel(m);
  c->SetView(v);

  v->SetController(c);
  v->SetModel(m);

  main_view->SetMVC(m,v,c);
}*/

Connection* BuddycloudService::CreateConnection() {
  return new BuddycloudConnection();
}
ServiceModelRef BuddycloudService::CreateMainModel() {
  return boost::shared_ptr<BModel>(new BModel);
}
ServiceViewRef BuddycloudService::CreateMainView() {
  return boost::shared_ptr<BView>(new BView);
}
ServiceController* BuddycloudService::CreateServiceController() {
  return new BController;
}
ServiceModel* BuddycloudService::CreateServiceModel(AccountData* account_data) {
  return new BuddycloudModel(account_data);
}
NewAccountWidget* BuddycloudService::CreateNewAccountPane() {
  return new BcNewAccountWidget;
}

std::vector<WidgetFactory*> BuddycloudService::CreateWidgetFactories() {
  std::vector<WidgetFactory*> factories;
  return factories;
}
