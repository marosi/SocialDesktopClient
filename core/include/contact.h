/**
 * @file contact.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Feb 11, 2012
 */

#ifndef CORE_CONTACT_H_
#define CORE_CONTACT_H_

#include "content.h"

namespace sdc {

class Contact : public Content {
 public:
  typedef boost::shared_ptr<Contact> Ref;

  void SetName(const std::string &name) {
    name_ = name;
  }

  void SetUid(const std::string &uid) {
    uid_ = uid;
  }

  void SetGroups(const std::vector<std::string> &groups) {
    groups_ = groups;
  }

  std::string GetName() {
    return name_;
  }

  std::string GetUid() {
    return uid_;
  }

 private:
  std::string name_;
  std::string uid_;
  std::vector<std::string> groups_;
};

class Contacts : public Content {
 public:
  typedef boost::shared_ptr<Contacts> Ref;

  Contacts() : ready_to_iterate_(false) {}

  void AddItem(Contact::Ref contact) {
    items_.push_back(contact);
  }

  void RemoveItem(Contact::Ref contact) {
    items_.remove(contact);
  }

  void Iterate() {
    iterator_ = items_.begin();
    ready_to_iterate_ = true;
  }

  Contact::Ref GetNext() {
    if(iterator_ == items_.end()) {
      ready_to_iterate_ = false;
      return Contact::Ref();
    }
    assert(ready_to_iterate_);
    Contact::Ref result = *iterator_;
    ++iterator_;
    return result;
  }

 private:
  bool ready_to_iterate_;
  std::list<Contact::Ref>::iterator iterator_;
  std::list<Contact::Ref> items_;
};

}  /* namespace sdc */



#endif /* CORE_CONTACT_H_ */
