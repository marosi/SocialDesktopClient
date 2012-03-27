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

template<class C>
class Items : public Content {
 public:
  typedef boost::shared_ptr<C> ContentRef;

  Items() : ready_to_iterate_(false) {}

  void AddItem(ContentRef content) {
    items_.push_back(content);
  }

  void RemoveItem(ContentRef content) {
    items_.remove(content);
  }

  void Iterate() {
    iterator_ = items_.begin();
    ready_to_iterate_ = true;
  }

  ContentRef GetNext() {
    if(iterator_ == items_.end()) {
      ready_to_iterate_ = false;
      return ContentRef();
    }
    assert(ready_to_iterate_);
    ContentRef result = *iterator_;
    ++iterator_;
    return result;
  }

 private:
  bool ready_to_iterate_;
  typename std::list<ContentRef>::iterator iterator_;
  std::list<ContentRef> items_;
};

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

class Contacts : public Items<Contact> {
 public:
  typedef boost::shared_ptr<Contacts> Ref;
};

class Post {
 public:
  typedef boost::shared_ptr<Post> Ref;

  void SetAuthor(const std::string author) {
    author_ = author;
  }

  void SetContent(const std::string &content) {
    content_ = content;
  }

  std::string GetAuthor() {
    return author_;
  }

  std::string GetContent() {
    return content_;
  }

 private:
  std::string content_;
  std::string author_;
};

class Posts : public Items<Post> {
 public:
  typedef boost::shared_ptr<Posts> Ref;
};

}  /* namespace sdc */

#endif /* CORE_CONTACT_H_ */
