/**
 * @file service_view.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 7, 2012
 */

#ifndef CORE_SERVICE_VIEW_H_
#define CORE_SERVICE_VIEW_H_

#include "view.h"
#include "boost/shared_ptr.hpp"

namespace sdc {

class ServiceView : public View {
 public:
  virtual void Update() {}
  template<class Controller>
  boost::shared_ptr<Controller> GetController() {
    return boost::dynamic_pointer_cast<Controller>(controller());
  }
};

}  /* namespace sdc */

#endif /* CORE_SERVICE_VIEW_H_ */
