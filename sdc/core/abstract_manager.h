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

#include "core_anchor.h"
#include "boost/noncopyable.hpp"

namespace sdc {

class Core;

class AbstractManager : public boost::noncopyable, public CoreAnchor {
 public:
  AbstractManager(Core* core) : CoreAnchor(core) {}

  virtual void OnExit() {}
};

}

#endif /* CORE_ABSTRACTMANAGER_H_ */
