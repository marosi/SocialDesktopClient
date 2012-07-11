/**
 * @file connection.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 31, 2012
 */

#ifndef CORE_CONNECTION_H_
#define CORE_CONNECTION_H_

#include "service.h"
#include "core_anchor.h"
#include <list>

namespace sdc {

class Service;
class ServiceController;

/// @class Connection
/// @brief TODO:comment
class Connection : public CoreAnchor {
 public:
  friend class ConnectionManager;

  virtual void Connect() = 0;
  virtual void Disconnect() = 0;

 private:
  /**
   * Connection thread method callback
   */
  void DoRun();
  virtual void Run() = 0;
};

} /* namespace sdc */

#endif /* CORE_CONNECTION_H_ */
