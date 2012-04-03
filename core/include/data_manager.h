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

namespace sdc {

class Core;

class DataManager : public AbstractManager {
 public:
  DataManager(Core* core) : AbstractManager(core) {}
};

} /* namespace sdc */

#endif /* CORE_DATA_MANAGER_H_ */
