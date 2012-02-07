/**
 * @file observer.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 9, 2012
 */

#ifndef CORE_OBSERVER_H_
#define CORE_OBSERVER_H_

#include "mvc_typedefs.h"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"

namespace sdc {

class Observer : public boost::enable_shared_from_this<Observer> {
 public:
  typedef boost::shared_ptr<Observer> Ref;

  virtual ~Observer();
  void SetModel(ModelRef model);
  virtual void Update()=0;

 private:
  ModelRef model_; // TODO: Unused so far.
};

} /* namespace sdc */

#endif /* CORE_OBSERVER_H_ */
