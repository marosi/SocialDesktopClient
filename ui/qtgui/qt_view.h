/**
 * @file qt_view.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 31, 2012
 */

#ifndef QT_VIEW_H_
#define QT_VIEW_H_

#include "view.h"
#include "core_anchor.h"
#include "qt_gui.h"
#include <QtGui/QWidget>
#include "boost/cast.hpp"

namespace sdc {

class Core;

class qtview : public QWidget, public View {

  Q_OBJECT

 public:
  qtview(QWidget* parent = 0);
  void Update();

 private slots:
  virtual void DoUpdate()=0;

 signals:
  void update();
};

class QtView : public CoreAnchor {
 public:
  QtView(QtGui* qt)
      : CoreAnchor(*qt), // called CoreAnchor copy constructor, therefore this view should have its own anchor to Core
        parent_(0),
        root_(qt) {}
  QtView(QtView* view)
      : CoreAnchor(*view),
        parent_(view),
        root_(parent_->qtgui()) {}
  QtView(QWidget* view)
      : CoreAnchor(*boost::polymorphic_cast<QtView*>(view)),
        parent_(boost::polymorphic_cast<QtView*>(view)),
        root_(parent_->qtgui()) {}
  virtual ~QtView() {}

  QtGui* qtgui() {
    return root_;
  }

  void SetFingerprint(ServiceModel* fingerprint) {
    fingerprint_ = fingerprint;
  }

  bool IsMatching(ServiceModel* fingerprint) {
    return fingerprint_ == fingerprint;
  }

 private:
  QtView* parent_;
  QtGui* root_;

  ServiceModel* fingerprint_;
};

template<class Controller, class Model>
class GenericQtView : public qtview {
 public:
  typedef boost::shared_ptr<Controller> Controller_ref;
  typedef boost::shared_ptr<Model> Model_ref;

  GenericQtView(QWidget* parent = 0) : qtview(parent) {}

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

#endif /* QT_VIEW_H_ */
