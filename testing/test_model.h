/**
 * @file test_model.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Jan 9, 2012
 */

#ifndef TEST_MODEL_H_
#define TEST_MODEL_H_

#include "model.h"

#include "log.h"
#include <string>

class TestModel : public sdc::Model {
 public:
  std::string GetContent() const {
    return content_;
  }
  void SetText(const std::string &content) {
    content_ = content;
    NotifyObservers();
    LOG(DEBUG2) << "Setting model content ... " << content_;
  }
 private:
  std::string content_;
};

#endif /* TEST_MODEL_H_ */
