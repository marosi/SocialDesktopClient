/**
 * @file test.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Nov 1, 2011
 */

#ifndef CORE_TESTING_TEST_H_
#define CORE_TESTING_TEST_H_

#include "core/include/common.h"

#include "core/include/core.h"

#include <string>

namespace sdc {

/*class Core;
class ExitController;

class UI {
 public:
  UI(Core* core) : core_(core) {}
  void Run() {
    std::string command;
    while(command != "exit") {
      std::cin >> command;
    }
    core_->Exit();
  }
 private:
  Core* core_;
};*/

/*
class Controller {
 public:
  Controller(void* model, void* view) : model_(model), view_(view) {}
  virtual void Execute()=0;
 protected:
  void* model_;
  void* view_;
};

class ExitController : public Controller {
 public:
  ExitController(const Core* core, void* view) : core_(core) {}
  void Execute() {
    core->Exit();
  }
};
*/

} // namespace sdc

#endif /* TEST_H_ */
