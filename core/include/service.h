/**
 * @file service.h
 * @brief TODO:comment
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Oct 28, 2011
 */

#ifndef CORE_SERVICE_H_
#define CORE_SERVICE_H_

#include "common.h"

#include <map>

namespace sdc {

// @class Service
/// @brief TODO:
class Service {
 public:
  class Connection;
  class UserConfig;

  Service();

  std::string name() { return name_; }
  std::string description() { return description_; }
  /// @brief Factory method for user connection.
  virtual UserConfig* CreateUserConfig(); // TODO: Prototype pattern for userconfig and connection?
  virtual Connection* CreateConnection()=0;

 protected:
  std::string name_;
  std::string description_;

};

/// @class Connection
/// @brief TODO:comment
class Service::Connection {
 public:
  virtual void Set(Service::UserConfig* /*user_config*/)=0;
  virtual void Run()=0;
};

/// @class UserConfig
/// @brief Holds data for specific user connection configuration.
class Service::UserConfig {
 public:
  UserConfig(Service* /*service*/);
  Service::Connection* CreateConnection();
  void SetTwo(const std::string &/*key*/, const std::string &/*value*/);

 private:
  Service* service_;
  std::map<std::string, std::string> twos;
};

}

#endif /* CORE_SERVICE_H_ */
