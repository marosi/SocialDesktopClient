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

#include "controller.h"
#include "core_anchor.h"
#include "qt_view.h"

namespace sdc {

class qtcontroller : public virtual Controller {
};

class QtController : public QObject, public qtcontroller {
 public:
  QtController() {
    LOG(WARNING)
        << "Controller object of type "
        << typeid(this).name()
        << " is without anchor to core so far!";
  }

  QtController(QtView* view) : CoreAnchor(*view) {

  }

  QWidget* GetView() const {
    return view_;
  }

 protected:
  /**
   * In order to add service view to application layout
   * child classes MUST set their QWidget
   */
  void SetControllersMainView(QWidget* view) {
    view_ = view;
  }

 private:
  QWidget* view_;
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
