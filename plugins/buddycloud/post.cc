/*
 * post.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 10, 2012
 */

#include "post.h"
#include "channel.h"
#include "Swiften/Base/DateTime.h"
#include <algorithm>

Post::Post(Channel* channel) : Content(channel), channel_(channel) {}

void Post::Delete() {
  channel_->DeletePost(this);
}

void Post::PostComment(const std::string &content) {
  channel_->PublishComment(GetID(), content);
}

Comment* Post::AddComment(Atom::ref atom, bool signal) {
  std::vector<Comment*>::iterator it =
      std::find_if(comments_.begin(), comments_.end(),
                   [&] (const Comment* c) { return c->GetID() == atom->getID(); });
  if (it != comments_.end()) {
    return *it;
  } else {
    Comment* cmt = new Comment(this);
    cmt->SetID(atom->getID());
    cmt->SetAuthor(atom->getAuthor());
    cmt->SetAuthorJID(atom->getAuthorJID());
    cmt->SetContent(atom->getContent());
    cmt->SetPublished(Swift::stringToDateTime(atom->getPublished()));
    comments_.push_back(cmt);
    if (signal)
      onCommentAdded(cmt);
    return cmt;
  }
}

Comment::Comment(Post* post) : Content(post), post_(post) {
  comment_on_id_ = post_->GetID();
}

const std::string& Comment::GetCommentedID() const {
  return comment_on_id_;
}
