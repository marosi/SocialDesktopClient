/**
 * @file model.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 4, 2012
 */

#ifndef CORE_MODEL_H_
#define CORE_MODEL_H_

#include <vector>
#include "boost/shared_ptr.hpp"

namespace sdc {

class Observer;

class Model {
 public:
  typedef boost::shared_ptr<Model> Ref;
  typedef boost::shared_ptr<Observer> ObserverRef;

  virtual ~Model() {}
  void RegisterObserver(ObserverRef observer);

 protected:
  void NotifyObservers() const;

  std::vector<ObserverRef> observers_;
};

} /* namespace sdc */

#endif /* CORE_MODEL_H_ */
