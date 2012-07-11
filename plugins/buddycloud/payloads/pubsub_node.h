/**
 * @file pubsub_node.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jul 2, 2012
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

#endif /* PUBSUB_NODE_H_ */
