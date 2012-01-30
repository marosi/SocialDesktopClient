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

#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"

namespace sdc {

class Model;

class Observer : public boost::enable_shared_from_this<Observer> {
 public:
  typedef boost::shared_ptr<Observer> Ref;

  Observer(boost::shared_ptr<Model> model);
  virtual ~Observer();
  void RegisterToModel();
  virtual void Update()=0;

 private:
  boost::shared_ptr<Model> model_; // TODO: Unused so far.
};

} /* namespace sdc */

#endif /* CORE_OBSERVER_H_ */
