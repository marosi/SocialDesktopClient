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
#include "mvc_typedefs.h"
#include "core_typedefs.h"
#include "boost/shared_ptr.hpp"
#include <map>

namespace sdc {

class Connection;
class ServiceController;

// @class Service
/// @brief TODO:
class Service {
 public:
  class UserConfig;

  Service();

  std::string name() { return name_; }
  std::string description() { return description_; }
  /// @brief Factory method for user connection.
  virtual UserConfig* CreateUserConfig(); // TODO: Prototype pattern for userconfig and connection?
  virtual Connection* CreateConnection()=0;
  virtual ServiceModelRef CreateMainModel()=0; // TODO: Delete, has no more sense
  virtual ServiceViewRef CreateMainView()=0; // TODO: Delete, has no more sense
  virtual ServiceController* CreateServiceController()=0;
  //virtual void InitializeGui(sdc::MainView* main_view)=0; // TODO: Delete, has no more sense

 protected:
  std::string name_;
  std::string description_;

};

/// @class UserConfig
/// @brief Holds data for specific user connection configuration.
class Service::UserConfig {
 public:
  UserConfig(Service* /*service*/);
  Connection* CreateConnection();
  void SetTwo(const std::string &/*key*/, const std::string &/*value*/);

 private:
  Service* service_;
  std::map<std::string, std::string> twos;
};

}

#endif /* CORE_SERVICE_H_ */
