/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef ABSTRACT_PRESENTER_H_
#define ABSTRACT_PRESENTER_H_

class BcPresenter;

class AbstractPresenter {
 public:
  AbstractPresenter(BcPresenter* presenter) : presenter_(presenter) {}
  AbstractPresenter(AbstractPresenter* presenter) : presenter_(presenter->presenter()) {}

 protected:
  BcPresenter* presenter() {
    return presenter_;
  }

 private:
  BcPresenter* presenter_;
};

#endif // ABSTRACT_PRESENTER_H_
