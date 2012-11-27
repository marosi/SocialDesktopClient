/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_CONTACT_H_
#define CORE_CONTACT_H_

#include "boost/shared_ptr.hpp"
#include "boost/signals2/signal.hpp"
#include <list>
#include <string>
#include <vector>

namespace sdc {

class Contact {
 public:
  typedef boost::shared_ptr<Contact> Ref;

  enum Status {
    Offline = 0,
    Online = 1
  };

  virtual ~Contact() {}

  void SetName(const std::string &name) { name_ = name; }
  const std::string& GetName() const { return name_; }
  void SetUid(const std::string &uid) { uid_ = uid; }
  const std::string& GetUid() const { return uid_; }
  void SetGroups(const std::vector<std::string> &groups) { groups_ = groups; }
  const std::vector<std::string>& GetGroups() const { return groups_; }
  void SetStatus(int status) {
    status_ = status;
    onStatusChanged(status_);
  }
  int GetStatus() const { return status_; }

  boost::signals2::signal<void (int status)> onStatusChanged;

 private:
  std::string name_;
  std::string uid_;
  std::vector<std::string> groups_;
  int status_;
};

} // namespace sdc

#endif // CORE_CONTACT_H_
