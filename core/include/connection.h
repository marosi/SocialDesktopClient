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
#include "core_typedefs.h"
#include <list>

namespace sdc {

class Message;
class Service;
class ServiceController;

/// @class Connection
/// @brief TODO:comment
class Connection : public CoreAnchor {
 public:
  friend class ConnectionManager;

  virtual void Connect() = 0;
  virtual void Disconnect() = 0;

  virtual void Send(RequestRef request);
  virtual void DeleteRequest(RequestRef request);

  /// @{ DEPRECATED
  bool IsActive() const {
    return is_active_;
  }
  void SetService(Service* service) {
    service_ = service;
  }
  void SetController(ServiceControllerRef ctrler) {
    controller_ = ctrler;
  }
  /// @}
  /// @{ DEPRECATED
  template<class Controller>
  boost::shared_ptr<Controller> GetController() {
      return boost::dynamic_pointer_cast<Controller>(controller_);
  }

  Service* service() const {
    return service_;
  }
  /// @}

 private:
  /**
   * Connection thread method callback
   */
  void DoRun();
  virtual void Run() = 0;

  Service* service_;
  boost::shared_ptr<ServiceController> controller_;
  bool is_active_;
  std::list<RequestRef> pending_requests_;
};

} /* namespace sdc */

#endif /* CORE_CONNECTION_H_ */
