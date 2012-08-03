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

#include "abstract_model.h"
#include "payloads/atom.h"
#include "Swiften/JID/JID.h"
#include "boost/signals2/signal.hpp"
#include "boost/date_time/posix_time/ptime.hpp"
#include <string>

class Channel;
class Comment;

class Content : public AbstractModel {
 public:
  Content(AbstractModel* model) : AbstractModel(model) {}

  void SetID(const std::string &id) { id_ = id; }
  std::string GetID() const { return id_; }
  void SetAuthor(const std::string &author) { author_ = author; }
  std::string GetAuthor() const { return author_; }
  void SetAuthorJID(const Swift::JID &jid) { author_jid_ = jid; }
  const Swift::JID& GetAuthorJID() const { return author_jid_; }
  void SetContent(const std::string &content) { content_ = content; }
  std::string GetContent() const { return content_; }
  void SetPublished(const boost::posix_time::ptime &published) { published_ = published; }
  boost::posix_time::ptime GetPublished() const { return published_; }

 private:
  std::string id_;
  std::string content_;
  std::string author_;
  Swift::JID author_jid_;
  boost::posix_time::ptime published_;
};

class Post : public Content {
 public:
  friend class Channel;
  friend class BcModel;

  Post(Channel* channel);
  void Delete();
  void PostComment(const std::string &content);

  std::vector<Comment*> comments() const {
    return comments_;
  }

  const Channel* channel() const {
    return channel_;
  }

  boost::signals2::signal<void (Comment*)> onCommentAdded;

 private:
  Comment* AddComment(Atom::ref atom, bool signal = true);

  Channel* channel_;
  std::vector<Comment*> comments_;
};

class Comment : public Content {
 public:
  Comment(Post* post);

  const std::string& GetCommentedID() const;

  const Post* post() const {
    return post_;
  }

 private:
  Post* post_;
  std::string comment_on_id_;
};


#endif /* POST_H_ */
