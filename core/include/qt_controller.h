/**
 * @file qt_controller.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 6, 2012
 */

#ifndef QT_CONTROLLER_H_
#define QT_CONTROLLER_H_

#include "service_controller.h"
#include <QObject>

namespace sdc {

class qtcontroller : public QObject, public ServiceController {

  Q_OBJECT

};

class QtController : public qtcontroller {
  // TODO: implemenet non-generic controller
};

template <class M, class V>
class GenericQtController : public qtcontroller {
 public:
  typedef boost::shared_ptr<M> Model_ref;
  typedef boost::shared_ptr<V> View_ref;

  void SetModel(Model_ref model) {
    model_ = model;
    Controller::SetModel(model);
  }
  void SetView(View_ref view) {
    view_ = view;
    Controller::SetView(view);
  }

 protected:
  Model_ref GetModel() { return model_; }
  View_ref GetView() { return view_; }

 private:
  Model_ref model_;
  View_ref view_;
};

} /* namespac sdc */

#endif /* QT_CONTROLLER_H_ */
