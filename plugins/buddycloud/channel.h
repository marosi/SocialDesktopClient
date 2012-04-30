/**
 * @file channel.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 23, 2012
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "sdc.h"
#include "post.h"
#include <string>
#include <map>
#include <set>

class ChannelController;

class Channel : public sdc::Content {
 public:
  typedef boost::shared_ptr<Channel> Ref;

  void SetContact(sdc::Contact::Ref contact) {
    contact_ = contact;
  }

  sdc::Contact::Ref GetContact() {
    return contact_;
  }

  void SetTitle(const std::string &title) {
    title_ = title;
  }

  std::string GetTitle() {
    return title_;
  }

  void SetDescription(const std::string &description) {
    description_ = description;
  }

  std::string GetDescription() {
    return description_;
  }

  void AddPost(Post1::Ref post) {
    std::pair<std::map<std::string, Post1::Ref>::iterator, bool> ret;
    ret = posts_.insert(std::pair<std::string, Post1::Ref>(post->GetID(), post));
    if (ret.second) {
      post->SetController(controller_);
      onPostAdded(post);
    }
  }

  void RemovePost(Post1::Ref post) {
    posts_.erase(post->GetID());
    post->Remove();
  }

  const std::set<Post1::Ref> GetPosts() const {
    std::set<Post1::Ref> result;
    std::map<std::string, Post1::Ref>::const_iterator it = posts_.begin();
    for (it = posts_.begin(); it != posts_.end(); ++it) {
      result.insert(it->second);
    }
    return result;
  }

  void SetController(ChannelController* controller) {
    controller_ = controller;
  }

  ChannelController* controller() {
    return controller_;
  }

  boost::signals2::signal1<void, Post1::Ref> onPostAdded;

 private:
  sdc::Contact::Ref contact_;
  std::string title_;
  std::string description_;
  std::map<std::string, Post1::Ref> posts_;

  ChannelController* controller_;
};



#endif /* CHANNEL_H_ */
