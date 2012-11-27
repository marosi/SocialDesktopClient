/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_CORE_ANCHOR_H_
#define CORE_CORE_ANCHOR_H_

#include <assert.h>

namespace sdc {

class Core;

class CoreAnchor {
 public:
  CoreAnchor() : core_(0) {} // TODO: Remove this together with SetCore() method
  CoreAnchor(Core* core) {
    core_ = core;
  }
  virtual ~CoreAnchor() {}

  void SetCore(Core* core) {// TODO: Modify so the core would be set in ctor in every manager.
    assert(core);
    core_ = core;
  }

  Core* GetCore() { return core_; }

 protected:
  Core* core()  {
    assert(core_);
    return core_;
  }

 private:
  Core* core_;
};

} // namespace sdc

#endif // CORE_CORE_ANCHOR_H_
