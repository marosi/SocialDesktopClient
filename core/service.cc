/*
 * service.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Oct 28, 2011
 */

#include "service.h"

namespace sdc {

Service::Service() : name_("Unknown"), description_("Unknown") { }

Service::UserConfig* Service::CreateUserConfig() {
  return new Service::UserConfig(this);
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
