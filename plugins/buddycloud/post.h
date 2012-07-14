/**
 * @file post.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Apr 23, 2012
 */

#ifndef POST_H_
#define POST_H_

#include "boost/signals2/signal.hpp"
#include <string>

class ChannelController;
class Comment;

class Content {
 public:

  void SetID(const std::string &id) {
    id_ = id;
  }

  std::string GetID() const {
    return id_;
  }

  void SetAuthor(const std::string &author) {
    author_ = author;
  }

  std::string GetAuthor() {
    return author_;
  }

  void SetContent(const std::string &content) {
    content_ = content;
  }

  std::string GetContent() {
    return content_;
  }

 private:
  std::string id_;
  std::string content_;
  std::string author_;
};

class Post1 : public Content {
 public:
  friend class ChannelController;
  friend class BcModel;

  Post1(ChannelController* channel);
  void Delete();
  void PostComment(const std::string &content);

  const std::vector<Comment*> comments() {
    return comments_;
  }

  boost::signals2::signal<void (Comment*)> onCommentAdded;

 private:
  void AddComment(Comment* comment, bool signal = true);

  ChannelController* channel_;
  std::vector<Comment*> comments_;
};

class Comment : public Content {
 public:
  Comment(Post1* post, const std::string &content);

  std::string GetCommentedID();

 private:
  Post1* post_;
  std::string comment_on_id_;
};


#endif /* POST_H_ */
