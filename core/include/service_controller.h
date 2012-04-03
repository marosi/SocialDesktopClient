/**
 * @file service_controller.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 6, 2012
 */

#ifndef CORE_SERVICE_CONTROLLER_H_
#define CORE_SERVICE_CONTROLLER_H_

#include "controller.h"
#include "core_typedefs.h"
#include "log.h"

namespace sdc {

class ServiceController : public virtual Controller {
 public:
  virtual ~ServiceController() {
    LOG(INFO) << "Turning the service controller off.";
  }

  void SetConnection(ConnectionRef conn) { connection_ = conn; }

  template<class Connection>
  boost::shared_ptr<Connection> GetConnection() {
      return boost::dynamic_pointer_cast<Connection>(connection_);
  }

  ConnectionRef GetConnection() {
    return connection_;
  }

 private:
  ConnectionRef connection_;
};

} /* namespace sdc */

#endif /* CORE_SERVICE_CONTROLLER_H_ */
