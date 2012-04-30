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
#include "buddycloud_bot.h"
#include "bc_new_account_widget.h"
#include "bc_contact.h"
#include "bc_contact_widget.h"
#include "bc_new_contact_widget.h"
#include "channel.h"
#include "channel_widget.h"
#include "post.h"
#include "post_widget.h"

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
  return new BuddycloudBot(account_data);
}
NewAccountWidget* BuddycloudService::CreateNewAccountPane() {
  return new BcNewAccountWidget;
}

NewContactWidget* BuddycloudService::CreateNewContactWidget() {
  return new BcNewContactWidget;
}

class ChannelWidgetFactory : public sdc::GenericWidgetFactory<Channel, ChannelWidget> {};
class PostWidgetFactory : public sdc::GenericWidgetFactory<Post1, PostWidget> {};
class ContactWidgetFactory : public sdc::GenericWidgetFactory<BcContact, BcContactWidget> {};

std::vector<WidgetFactory*> BuddycloudService::CreateWidgetFactories() {
  std::vector<WidgetFactory*> factories;
  factories.push_back(new ChannelWidgetFactory);
  factories.push_back(new PostWidgetFactory);
  factories.push_back(new ContactWidgetFactory);
  return factories;
}
