/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef PUBSUB_NODE_H_
#define PUBSUB_NODE_H_

#include "Swiften/Elements/Payload.h"

class PubsubNode : public Swift::Payload {
 public:
  typedef boost::shared_ptr<PubsubNode> ref;

  void setNode(const std::string &node) {
    node_ = node;
  }

  std::string getNode() {
    return node_;
  }

 private:
  std::string node_;
};

#endif // PUBSUB_NODE_H_
