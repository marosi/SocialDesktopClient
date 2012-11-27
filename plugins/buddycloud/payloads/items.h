/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
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

#endif // ITEMS_H_
