#ifndef ABSTRACT_PRESENTER_H
#define ABSTRACT_PRESENTER_H

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

#endif // ABSTRACT_PRESENTER_H
