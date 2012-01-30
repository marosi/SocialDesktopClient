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
#include "boost/shared_ptr.hpp"

namespace sdc {

class Model;

/**
 * View base class.
 *
 * TODO: View vs. Observer ...there's not much of a use of both.
 */
class View : public Observer {
 public:
  typedef boost::shared_ptr<View> Ref;
  View(boost::shared_ptr<Model> model) : Observer(model) {}
};

/**
 * Generic view.
 *
 * Need to know interface of model and controller.
 */
template<class Controller, class Model>
class GenericView : public View {
 public:
  typedef boost::shared_ptr<Controller> ControllerRef;
  typedef boost::shared_ptr<Model> ModelRef;

  GenericView(ControllerRef controller, ModelRef model) :
      View(model),
      controller_(controller),
      model_(model) {}

 protected:
  ControllerRef GetController() { return controller_; }
  ModelRef GetModel() { return model_; }

 private:
  ControllerRef controller_;
  ModelRef model_;
};

} /* namespace sdc */

#endif /* CORE_VIEW_H_ */
