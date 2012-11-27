/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_SERVICE_H_
#define CORE_SERVICE_H_

#include "common.h"
#include "boost/shared_ptr.hpp"

namespace sdc {

class ServiceController;
class ServiceModel;
class Account;

/// @class Service
/// @brief TODO:
class Service {
 public:
  friend class Core;

  Service() : name_("Unknown"), description_("Unknown") {}

  std::string name() {
    return name_;
  }

  std::string description() {
    return description_;
  }

  const std::string dir() {
    return dir_path_;
  }

  std::string signature() {
    return signature_;
  }

  virtual ServiceModel* CreateServiceModel(Account*)=0;

 protected:
  void SetName(const std::string &name) {
    name_ = name;
  }

  void SetDescription(const std::string &description) {
    description_ = description;
  }

  void SetDir(const std::string &dir_path) {
    dir_path_ = dir_path;
  }

 private:
  std::string signature_;
  std::string name_;
  std::string description_;
  std::string dir_path_;
};

} // namespace sdc

#endif // CORE_SERVICE_H_
