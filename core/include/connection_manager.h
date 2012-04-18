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
#include "core_typedefs.h"
#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"
#include <vector>

namespace sdc {

class Connection;
class Core;
class Service;

/// @class ConnectionManager
/// @brief Maintains everything to do with the services connections.
/// Initiates each user service connection and maintains it.
class ConnectionManager : public AbstractManager {
 public:
  ConnectionManager(Core* core) : AbstractManager(core) {}

  void InitServiceConnections();
  void ConnectAll();
  void GetAllActiveConnections(std::vector<ConnectionRef> &result) const;

  void MakeConnection(Connection* connection);

 private:
  void RemoveInactiveConnections() {} // TODO: impelment


  std::vector<ConnectionRef> connections_old; // TODO: remove
  std::vector<Connection*> connections_;
  boost::thread_group threads_;
};

} /* namespace sdc */

#endif /* CORE_CONNECTIONMANAGER_H_ */
