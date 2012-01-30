/*
 * service.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Oct 28, 2011
 */

#include "service.h"
#include "message.h"
#include "core.h"
#include "log.h"
#include "boost/bind.hpp"

namespace sdc {

Service::Service() : name_("Unknown"), description_("Unknown") { }

Service::UserConfig* Service::CreateUserConfig() {
  return new Service::UserConfig(this);
}

void Service::Connection::DoRun() {
  is_active_ = true;
  LOG(DEBUG) << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CONNECTION ACTIVE";
  Run();
  LOG(DEBUG) << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CONNECTION INACTIVE";
  is_active_ = false;
}

void Service::Connection::RecieveMessage(boost::shared_ptr<Message> message) {
  core()->event_manager()->PostEvent(boost::bind(&Core::Process, core(), message));
}

Service::UserConfig::UserConfig(Service* service) {
  service_ = service;
}

Service::Connection* Service::UserConfig::CreateConnection() {
  Connection* c = service_->CreateConnection();
  c->Set(this);
  return c;
}

void Service::UserConfig::SetTwo(const std::string &key, const std::string &value) {
  twos.insert(std::pair<std::string, std::string>(key, value));
}

}
