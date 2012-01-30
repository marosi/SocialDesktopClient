/**
 * @file connection_manager.h
 * @brief SDC service/protocol connection interface.
 *
 * @author Maros Kasinec
 *   @date Jul 10, 2011
 */

#ifndef CORE_CONNECTIONMANAGER_H_
#define CORE_CONNECTIONMANAGER_H_

#include "common.h"
#include "abstract_manager.h"

#include "config_manager.h"
#include "service.h"
#include <iostream>
#include <vector>
#include "boost/thread.hpp"


namespace sdc {

/// @class ConnectionManager
/// @brief Maintains everything to do with the services connections.
/// Initiates each user service connection and maintains it.
class ConnectionManager : public AbstractManager {
 public:
  void ConnectAll();
  std::vector<Service::Connection*> GetAllActiveConnections() const;

 private:
  /// @brief TODO:
  void MakeConnection(Service::UserConfig* /*user_config*/);
  void RemoveInactiveConnections() {} // TODO: impelment


  std::vector<Service::Connection*> connections_;
  boost::thread_group threads_;
};

} // namespace sdc

#endif /* CORE_CONNECTIONMANAGER_H_ */
