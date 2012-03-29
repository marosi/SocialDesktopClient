/**
 * @file content.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 12, 2012
 */

#ifndef CORE_CONTENT_H_
#define CORE_CONTENT_H_

#include "boost/shared_ptr.hpp"
#include <string>

namespace sdc {

class Content {
 public:
  typedef boost::shared_ptr<Content> Ref;

  void SetID(const std::string &id) {
    id_ = id;
  }

  std::string GetID() {
    return id_;
  }

 private:
  std::string id_;
};

}  /* namespace sdc */

#endif /* CORE_CONTENT_H_ */
