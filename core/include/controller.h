/**
 * @file controller.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Dec 6, 2011
 */

#ifndef CORE_CONTROLLER_H_
#define CORE_CONTROLLER_H_

#include "mvc_typedefs.h"
#include "core_anchor.h"
#include "boost/make_shared.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"

namespace sdc {

class Controller : public CoreAnchor {
 public:
  Controller() {}
  void SetModel(ModelRef model) {
    model_ = model;
  }
  void SetView(ViewRef view) {
    view_ = view;
  }
  template<class Model>
  boost::shared_ptr<Model> GetModel() {
    return boost::dynamic_pointer_cast<Model>(model_);
  }
  virtual ~Controller() {}
  virtual void Initiate() {}

 private:
  ModelRef model_;
  ViewRef view_;
};

/**
 * Generic controller class.
 *
 * Binds model and view within the controller.
 */
template <class M, class V>
class GenericController : public Controller {
 public:
  typedef boost::shared_ptr<M> Model_ref;
  typedef boost::shared_ptr<V> View_ref;

  void SetModel(Model_ref model) {
    model_ = model;
    Controller::SetModel(model);
  }
  void SetView(View_ref view) {
    view_ = view;
    Controller::SetView(view);
  }

 protected:
  Model_ref GetModel() { return model_; }
  View_ref GetView() { return view_; }

 private:
  Model_ref model_;
  View_ref view_;
};


template <class M, class V, class C>
class GenericAutoController : public boost::enable_shared_from_this<C>, public Controller {
 public:
  typedef boost::shared_ptr<M> Model_ref;
  typedef boost::shared_ptr<V> View_ref;

  GenericAutoController(Model_ref model, View_ref view) :
      model_(model),
      view_(view)
  {
    SetModel(model);
    SetView(view);
  }
  virtual void Init() {
    //view_ = boost::make_shared<ViewRef>(this->shared_from_this(), GetModel());
    View_ref tmp(new V(this->shared_from_this(), GetModel()));
    view_ = tmp;
  }

 protected:
  Model_ref GetModel() { return model_; }
  View_ref GetView() { return view_; }

 private:
  /*virtual ViewRef CreateView() {
    ViewRef ref(new View(this->shared_from_this(), GetModel())); // PROBLEM: shared_from_this() returns templated class specified in boost::enable_shared_from_this<TheClass>
    return ref;
    //return boost::make_shared<ViewRef>(this->shared_from_this(), GetModel());
  }*/

  Model_ref model_;
  View_ref view_;
};

/** TODO: GENERIC SERVICE CONTROLLER
 * Generic service controller class.
 *
 * A type of service is added to controller in order to work with certain service connection.
 */
/*template <class Model, class View, class Service>
class GenericServiceController : public GenericController<Model, View> {
 public:
  typedef boost::shared_ptr<Service> ServiceRef;

 protected:
  ServiceRef GetConnection() { return connection_; }
  void SetConnection(ServiceRef connection) { connection_ = connection; }

 private:
  ServiceRef connection_;
};*/

/*
// TODO: move to separate file
#include <QObject>

class TestController : public GenericController<DataManager, TestWindow>, public QObject {

  Q_OBJECT

 public:
  TestController(DataManager* model, TestWindow* view);
  void SetView(QWidget* view) {
    view_ = view;
  }
  QWidget* GetView() {
    return view_;
  }
  void Update(std::string content);
 signals:
  void update(QString content);
 private:
  void Init();

  TestWindow* test_window_;
  DataManager*  data_manager_;
};*/

} /* namespace sdc */

#endif /* CORE_CONTROLLER_H_ */
