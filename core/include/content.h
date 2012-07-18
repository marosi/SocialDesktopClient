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
#include "boost/signals2/signal.hpp"
#include <list>
#include <string>
#include <vector>

namespace sdc {

class Contact {
 public:
  typedef boost::shared_ptr<Contact> Ref;

  virtual ~Contact() {}

  void SetName(const std::string &name) {
    name_ = name;
  }

  void SetUid(const std::string &uid) {
    uid_ = uid;
  }

  void SetGroups(const std::vector<std::string> &groups) {
    groups_ = groups;
  }

  const std::vector<std::string>& GetGroups() const {
    return groups_;
  }

  const std::string& GetName() const {
    return name_;
  }

  const std::string& GetUid() const {
    return uid_;
  }

 private:
  std::string name_;
  std::string uid_;
  std::vector<std::string> groups_;
};

}  /* namespace sdc */

#endif /* CORE_CONTENT_H_ */
