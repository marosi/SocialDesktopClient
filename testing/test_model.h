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
#include <vector>

class TestModel : public sdc::Model {
 public:
  std::vector<std::string> GetContent() const {
    return content_;
  }
  std::string GetLastlyAddedContent() const {
    return content_.back();
  }
  void SetText(const std::string &content) {
    content_.push_back(content);
    NotifyObservers();
  }
 private:
  std::vector<std::string> content_;
};

#endif /* TEST_MODEL_H_ */
