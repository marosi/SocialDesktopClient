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

#include "core_anchor.h"
#include "qt_gui.h"
#include <QtGui/QWidget>
#include "boost/cast.hpp"

namespace sdc {

class Core;

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
      : CoreAnchor(0) {
    if (view) {
      parent_ = boost::polymorphic_cast<QtView*>(view);
      root_ = parent_->qtgui();
      SetCore(root_->GetCore());
    }
  }
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

} /* namespace sdc */

#endif /* QT_VIEW_H_ */
