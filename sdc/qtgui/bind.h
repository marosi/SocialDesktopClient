/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef BIND_H_
#define BIND_H_

#include <QObject>
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/make_shared.hpp"
#include "boost/signals2/signal.hpp"
#include <type_traits>

namespace sdc {

class bind_params_base : public QObject {

    Q_OBJECT

 public:
  virtual void callback()=0;
};

template<typename R, typename P>
class bind_params : public bind_params_base {
 public:
  bind_params(boost::function<R (P)> f, P p) : f_(f), p_(p) {}
  void callback() {
    f_(p_);
  }
 private:
  boost::function<R (P)> f_;
  P p_;
};

class bind_base : public QObject {

    Q_OBJECT

 public:
  bind_base() {

  }

 protected:
  virtual void callback() {}

 signals:
  void signal();
  void signal(bind_params_base*);

 private slots:
  void slot() {
    callback();
  }
  void slot(bind_params_base* p) {
    p->callback();
  }
};

template<typename Signature>
class bind_;

template<typename R>
class bind_<R ()>: public bind_base {
 public:
  bind_(boost::function<R ()> callback) : bind_base() {
    callback_ = callback;
    QObject::connect(this, SIGNAL(signal()), this, SLOT(slot()));
  }

  void emit_signal() {
    emit signal();
  }

  void connect(boost::signals2::signal<R ()> &signal, boost::shared_ptr<bind_<R ()> > ptr) {
    signal.connect(boost::bind(&bind_<R ()>::emit_signal, ptr));
  }

 protected:
  void callback() {
   callback_();
  }

 private:
  boost::function<R ()> callback_;
};

template<typename R, typename P>
class bind_<R (P)> : public bind_base {
 public:
  bind_(boost::function<R (P)> callback) : bind_base() {
    callback_ = callback;
    QObject::connect(this, SIGNAL(signal(bind_params_base*)), this, SLOT(slot(bind_params_base*)));
  }

  void emit_signal(P p) {
    bind_params<R, P>* hook = new bind_params<R, P>(callback_, p);
    emit signal(hook);
  }

  void connect(boost::signals2::signal<R (P)> &signal, boost::shared_ptr<bind_<R (P)> > ptr) {
    signal.connect(boost::bind(&bind_<R (P)>::emit_signal, ptr, _1));
  }

 private:
  boost::function<R (P)> callback_;
};

template<typename T>
void bind(boost::signals2::signal<T> &signal, typename std::common_type<boost::function<T> >::type function) {
  boost::shared_ptr<bind_<T> > binding(new bind_<T>(function));
  binding->connect(signal, binding);
}

} // namespace sdc

#endif // BIND_H_
