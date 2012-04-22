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
#include "boost/signals2.hpp"
#include <list>
#include <string>
#include <vector>

namespace sdc {

class ServiceModel;

class Content {
 public:
  typedef boost::shared_ptr<Content> Ref;

  virtual ~Content() {}

  void SetID(const std::string &id) {
    id_ = id;
    onChange();
  }

  std::string GetID() const {
    return id_;
  }

  void SetViewable(bool is_viewable) {
    is_viewable_ = is_viewable;
    onChange();
  }

  bool IsViewable() {
    return is_viewable_;
  }

  void SetServiceModel(ServiceModel* model) {
    model_ = model;
    onChange();
  }

  ServiceModel* GetServiceModel() {
    return model_;
  }

  void Remove() {
    onRemove();
  }

  boost::signals2::signal0<void> onChange;
  boost::signals2::signal0<void> onRemove;

 private:
  std::string id_;
  bool is_viewable_;
  ServiceModel* model_;
};

template<class C>
class Items : public Content {
 public:
  typedef boost::shared_ptr<Items<C> > Ref;
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

  void Clear() {
    typename std::list<ContentRef>::iterator it;
    for (it = items_.begin(); it != items_.end(); ++it) {
      it->reset();
    }
  }

 private:
  bool ready_to_iterate_;
  typename std::list<ContentRef>::iterator iterator_;
  std::list<ContentRef> items_;
};

class Contact : public Content {  // TODO: Contact should'n be treated as content... Subject class?
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

class Post : public Content {
 public:
  typedef boost::shared_ptr<Post> Ref;

  void SetAuthor(const std::string &author) {
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
  std::string id_;
};

}  /* namespace sdc */

#endif /* CORE_CONTENT_H_ */
