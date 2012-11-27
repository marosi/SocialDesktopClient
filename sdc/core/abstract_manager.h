/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_ABSTRACTMANAGER_H_
#define CORE_ABSTRACTMANAGER_H_

#include "core_anchor.h"
#include "boost/noncopyable.hpp"

namespace sdc {

class Core;

/**
 * An ancestor for all managers.
 */
class AbstractManager : public boost::noncopyable, public CoreAnchor {
 public:
  AbstractManager(Core* core) : CoreAnchor(core) {}

  virtual void OnExit() {}
};

} // namespace sdc

#endif // CORE_ABSTRACTMANAGER_H_
