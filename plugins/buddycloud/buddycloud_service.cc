/*
 * buddycloud_service.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Feb 7, 2012
 */

#include "buddycloud_service.h"
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

Connection* BuddycloudService::CreateConnection() {
  return new BuddycloudConnection();
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
