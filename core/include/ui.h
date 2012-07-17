/**
 * @file gui.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 31, 2012
 */

#ifndef GUI_H_
#define GUI_H_

#include "core_anchor.h"

namespace sdc {

class Service;
class ServiceController;

class UI : public CoreAnchor { // TODO: Abstract manager holds anchor to Core, should Ui be Manager or should AbstractManager be kind of Core referent
 public:
  UI(Core* core);
  virtual ~UI();

  /**
   * @brief GUI initializes in this method.
   */
  virtual void Init();

  /**
   * GUI execution point.
   *
   * This method is responsible for keeping GUI alive as long as it takes.
   */
  virtual int Exec()=0;
};

} /* namespace sdc */

#endif /* GUI_H_ */
