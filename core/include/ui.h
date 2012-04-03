/**
 * @file gui.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 31, 2012
 */

#ifndef GUI_H_
#define GUI_H_

#include "core_anchor.h"
#include "controller.h"
#include <vector>

namespace sdc {

class Service;
class ServiceController;

class UI : public CoreAnchor { // TODO: Abstract manager holds anchor to Core, should Ui be Manager or should AbstractManager be kind of Core referent
 public:
  UI(Core* core) : CoreAnchor(core) {}
  ~UI();

  virtual void Init();

  /**
   * GUI execution point.
   *
   * This method is responsible for keeping GUI alive as long as it takes.
   */
  virtual int Exec()=0;

 protected:
  const std::vector<ServiceController*>& GetControllers() const {
    return controllers_;
  }

  virtual ServiceController* CreateServiceController(Service* service);
  template<class C>
  C* CreateController() {
    C* result = new C;
    Controller* controller = result;
    controller->SetCore(this->GetCore());
    return result;
  }

 private:
  std::vector<ServiceController*> controllers_;
};

} /* namespace sdc */

#endif /* GUI_H_ */
