/**
 * @file data_manager.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Dec 6, 2011
 */

#ifndef CORE_DATA_MANAGER_H_
#define CORE_DATA_MANAGER_H_

#include "abstract_manager.h"
#include "common.h"
#include <vector>
#include "model.h"

namespace sdc {

class DataManager : public AbstractManager, public Model {
 public:
  DataManager();
  void AddContent(std::string content);

 private:
  /**
   * Mechanism for notifying GUI
   *
   * At this point SDC core is interconnected with GUI.
   */
  void Notify();

 private:
  std::vector<std::string> main_content_;
};

} /* namespace sdc */

#endif /* CORE_DATA_MANAGER_H_ */
