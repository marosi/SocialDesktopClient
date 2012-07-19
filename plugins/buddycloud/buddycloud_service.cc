/*
 * buddycloud_service.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Feb 7, 2012
 */

#include "buddycloud_service.h"
#include "bc_model.h"
#include "bc_new_account_widget.h"
#include "bc_contact_widget.h"
#include "bc_new_contact_widget.h"
#include "bc_presenter.h"
#include "channel_widget.h"
#include "post_widget.h"

using namespace sdc;

ServiceModel* BuddycloudService::CreateServiceModel(AccountData* account_data) {
  return new BcModel(account_data);
}

ServicePresenter* BuddycloudService::CreateServicePresenter() {
  return new BcPresenter();
}

NewAccountWidget* BuddycloudService::CreateNewAccountPane() {
  return new BcNewAccountWidget;
}

NewContactWidget* BuddycloudService::CreateNewContactWidget() {
  return new BcNewContactWidget;
}

//class ChannelWidgetFactory : public sdc::GenericWidgetFactory<Channel, ChannelWidget> {};
//class PostWidgetFactory : public sdc::GenericWidgetFactory<Post1, PostWidget> {};
//class ContactWidgetFactory : public sdc::GenericWidgetFactory<BcContact, BcContactWidget> {};

std::vector<WidgetFactory*> BuddycloudService::CreateWidgetFactories() {
  std::vector<WidgetFactory*> factories;
  //factories.push_back(new ChannelWidgetFactory);
  //factories.push_back(new PostWidgetFactory);
  //factories.push_back(new ContactWidgetFactory);
  return factories;
}
