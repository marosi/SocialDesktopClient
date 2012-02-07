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
#include "core_typedefs.h"

namespace sdc {

class Message;
class Core;
class ServiceController;

/// @class Connection
/// @brief TODO:comment
class Connection {
 public:
  friend class ConnectionManager;

  bool IsActive() const { return is_active_; }

  virtual void Set(Service::UserConfig* /*user_config*/)=0;
  virtual void SendMessage(boost::shared_ptr<Message> message)=0;
  virtual void RecieveMessage(boost::shared_ptr<Message> message);
  void SetService(Service* service) { service_ = service; }
  void SetController(ServiceControllerRef ctrler) { controller_ = ctrler; }
  template<class Controller>
  boost::shared_ptr<Controller> GetController() {
      return boost::dynamic_pointer_cast<Controller>(controller_);
  }

  Service* service() { return service_; }

 protected:
  Core* core() const { return core_; }

 private:
  void SetCore(Core* core) { core_ = core; }
  void DoRun();
  virtual void Run()=0;

  Core* core_;
  Service* service_;
  boost::shared_ptr<ServiceController> controller_;
  bool is_active_;
};

} /* namespace sdc */

#endif /* CONNECTION_H_ */
