/*
 * post.cc
 *
 *			Author: Maros Kasinec
 *  Created on: Jul 10, 2012
 */

#include "post.h"
#include "channel.h"
#include <algorithm>

Post1::Post1(ChannelController* channel) : Content(channel), channel_(channel) {}

void Post1::Delete() {
  channel_->DeletePost(this);
}

void Post1::PostComment(const std::string &content) {
  Comment* c = new Comment(this, content);
  channel_->CreateComment(c);
}

void Post1::AddComment(Comment* comment, bool signal) {
  std::vector<Comment*>::iterator it =
      std::find_if(comments_.begin(), comments_.end(),
                   [&] (const Comment* c) { return c->GetID() == comment->GetID(); });
  if (it == comments_.end()) {
    comments_.push_back(comment);
    if (signal)
      onCommentAdded(comment);
  }
}

Comment::Comment(Post1* post, const std::string &content) : Content(post), post_(post) {
  comment_on_id_ = post->GetID();
  SetContent(content);
}

std::string Comment::GetCommentedID() {
  return comment_on_id_;
}
