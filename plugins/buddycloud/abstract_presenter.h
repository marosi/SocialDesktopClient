/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef ABSTRACT_PRESENTER_H_
#define ABSTRACT_PRESENTER_H_

class BcPresenter;

/**
 * Common presenter class.
 * All descendants has access to main ServicePresenter instance.
 */
class AbstractPresenter {
 public:
  AbstractPresenter(BcPresenter* presenter) : presenter_(presenter) {}
  AbstractPresenter(AbstractPresenter* presenter) : presenter_(presenter->presenter()) {}

 protected:
  /**
   * Gets buddycloud service presenter.
   * @return service presenter
   */
  BcPresenter* presenter() {
    return presenter_;
  }

 private:
  BcPresenter* presenter_;
};

#endif // ABSTRACT_PRESENTER_H_
