/**
 * @file main_view.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 6, 2012
 */

#ifndef CORE_MAIN_VIEW_H_
#define CORE_MAIN_VIEW_H_

#include "view.h"
#include "mvc_typedefs.h"

namespace sdc {

class MainView : public View {
 public:
  virtual void Update() {}
  void SetMVC(ModelRef model, ViewRef view, ControllerRef controller) {
    model_ = model; view_ = view; controller_ = controller;
  }
 private:
  //std::vector<ModelRef>
  ModelRef model_;
  ViewRef view_;
  ControllerRef controller_;
};

} /* namespace sdc */

#endif /* CORE_MAIN_VIEW_H_ */
