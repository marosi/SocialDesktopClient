/**
 * @file view.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 4, 2012
 */

#ifndef CORE_VIEW_H_
#define CORE_VIEW_H_

#include "observer.h"
#include "mvc_typedefs.h"
#include "boost/shared_ptr.hpp"

namespace sdc {

/**
 * View base class.
 *
 * TODO: View vs. Observer ...there's not much of a use of both.
 */
class View : public Observer {
 public:
  View() {}
  void SetController(ControllerRef controller) {
    controller_ = controller;
  }
  ControllerRef controller() { return controller_; }
 private:
  ControllerRef controller_;
};

/**
 * Generic view.
 *
 * In order to work with controller and model, view has to know their interfaces.
 * This template gets work done for its child classes.
 */
template<class Controller, class Model>
class GenericView : public View {
 public:
  typedef boost::shared_ptr<Controller> Controller_ref;
  typedef boost::shared_ptr<Model> Model_ref;

  void SetModel(Model_ref model) {
    View::SetModel(model);
    model_ = model;
  }
  void SetController(Controller_ref controller) {
    View::SetController(controller);
    controller_ = controller;
  }

 protected:
  Controller_ref GetController() { return controller_; }
  Model_ref GetModel() { return model_; }

 private:
  Controller_ref controller_;
  Model_ref model_;
};

} /* namespace sdc */

#endif /* CORE_VIEW_H_ */
