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

class AbstractManager : public boost::noncopyable {
 public:
  virtual void OnExit() {}

};

}

#endif /* CORE_ABSTRACTMANAGER_H_ */
