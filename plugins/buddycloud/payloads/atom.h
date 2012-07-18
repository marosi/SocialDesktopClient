/**
 * @file atom.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jul 2, 2012
 */

#ifndef ATOM_H_
#define ATOM_H_

#include "Swiften/Elements/Payload.h"
#include "Swiften/JID/JID.h"
#include "boost/shared_ptr.hpp"
#include <string>

class Atom : public Swift::Payload {
 public:
  friend class AtomSerializer;
  typedef boost::shared_ptr<Atom> ref;
  enum Verb { Post };
  enum ObjectType { Note, Comment };

  Atom() {}

  void setID(const std::string & id) {
    id_ = id;
  }

  void setVerb(Verb verb) {
    verb_ = verb;
  }

  void setObjectType(ObjectType type) {
    object_type_ = type;
  }

  void setPublished(const std::string & published) {
    published_ = published;
  }

  void setAuthor(const std::string & author) {
    author_ = author;
  }

  void setContent(const std::string & content) {
    content_ = content;
  }

  std::string getID() const {
    return id_;
  }

  Verb getVerb() const {
    return verb_;
  }

  ObjectType getObjectType() const {
    return object_type_;
  }

  const std::string getVerbString() const {
    switch (verb_) {
      case Post:
        return "post";
      default:
        assert(false);
        break;
    }
    return "#UNDEFINED#";
  }

  const std::string getObjectTypeString() const {
    switch (object_type_) {
      case Note:
        return "note";
      case Comment:
        return "comment";
      default:
        assert(false);
        break;
    }
    return "#UNDEFINED#";
  }

  const std::string& getPublished() const {
    return published_;
  }

  const std::string& getAuthor() const {
    return author_;
  }

  const std::string& getContent() const {
    return content_;
  }

  void setInReplyTo(const std::string &thread_id) {
    thread_id_ = thread_id;
  }

  const std::string& getInReplyTo() const {
    return thread_id_;
  }

  void setUpdated(const std::string &updated) { updated_ = updated; }
  const std::string& getUpdated() const { return updated_; }

  void setAuthorJID(const Swift::JID &jid) { author_jid_ = jid; }
  const Swift::JID& getAuthorJID() const { return author_jid_; }

 private:
  Verb verb_;
  ObjectType object_type_;
  std::string published_;
  std::string updated_;
  std::string author_;
  Swift::JID author_jid_;
  std::string content_;
  std::string id_;
  std::string thread_id_;
};

#endif /* ATOM_H_ */
