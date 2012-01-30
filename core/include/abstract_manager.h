/**
 * @file abstract_manager.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Oct 31, 2011
 */

#ifndef CORE_ABSTRACTMANAGER_H_
#define CORE_ABSTRACTMANAGER_H_

#include "boost/noncopyable.hpp"

namespace sdc {

class Core;

class AbstractManager : public boost::noncopyable {
 public:
  virtual void OnExit() {}

  void SetCore(Core* core) { // TODO: Modify so the core would be set in ctor in every manager.
    core_ = core;
  }

 protected:
  Core* GetCore() { return core_; }

 private:
  Core* core_;
};

}

#endif /* CORE_ABSTRACTMANAGER_H_ */
