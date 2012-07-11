/**
 * @file items.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jul 2, 2012
 */

#ifndef ITEMS_H_
#define ITEMS_H_

#include "Swiften/Elements/Payload.h"
#include "boost/shared_ptr.hpp"
#include <vector>

template<class P>
class Items : public Swift::Payload {
 public:
  typedef boost::shared_ptr<Items<P> > ref;

  void appendPayload(boost::shared_ptr<P> payload) {
    items_.push_back(payload);
  }

  const std::vector<boost::shared_ptr<P> >& get() const {
    return items_;
  }

 private:
  std::vector<boost::shared_ptr<P> > items_;
};

#endif /* ITEMS_H_ */
