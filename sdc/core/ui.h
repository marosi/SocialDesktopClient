/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_UI_H_
#define CORE_UI_H_

namespace sdc {

class Service;
class ServiceController;

class UI {
 public:
  UI();
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

} // namespace sdc

#endif // CORE_UI_H_
