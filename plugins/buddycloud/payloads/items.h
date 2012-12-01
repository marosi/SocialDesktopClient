/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef ITEMS_H_
#define ITEMS_H_

#include "Swiften/Elements/Payload.h"
#include "boost/shared_ptr.hpp"
#include "boost/foreach.hpp"
#include <vector>

class Items : public Swift::Payload {
 public:
  typedef boost::shared_ptr<Items> ref;

  void appendPayload(Swift::Payload::ref payload) {
    items_.push_back(payload);
  }

  const std::vector<Swift::Payload::ref >& get() const {
    return items_;
  }

  template<class T>
  const std::vector<boost::shared_ptr<T> > getInternal() const {
    std::vector<boost::shared_ptr<T> > result;
    BOOST_FOREACH (Swift::Payload::ref payload, items_) {
      result.push_back(boost::dynamic_pointer_cast<T>(payload));
    }
    return result;
  }

 private:
  std::vector<Swift::Payload::ref > items_;
};

#endif // ITEMS_H_
