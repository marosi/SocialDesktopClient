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

#include "model.h"
#include "view.h"
#include "boost/make_shared.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"

namespace sdc {

class Controller {
 public:
  typedef boost::shared_ptr<Controller> Ref;

  Controller() {}
  virtual ~Controller() {}
  virtual void Init() {};
};

/**
 * Generic controller class.
 *
 * Binds model and view within the controller. TODO: Make model and view type safe, now any class can be model or view
 */
template <class M, class V, class C>
class GenericController : public boost::enable_shared_from_this<C>, public Controller {
 public:
  typedef boost::shared_ptr<M> ModelRef;
  typedef boost::shared_ptr<V> ViewRef;

  GenericController(ModelRef model) : model_(model) {}
  virtual void Init() {
    //view_ = boost::make_shared<ViewRef>(this->shared_from_this(), GetModel());
    ViewRef tmp(new V(this->shared_from_this(), GetModel()));
    tmp->RegisterToModel();
    view_ = tmp;
  }

 protected:
  ModelRef GetModel() { return model_; }
  ViewRef GetView() { return view_; }

 private:
  /*virtual ViewRef CreateView() {
    ViewRef ref(new View(this->shared_from_this(), GetModel())); // PROBLEM: shared_from_this() returns templated class specified in boost::enable_shared_from_this<TheClass>
    return ref;
    //return boost::make_shared<ViewRef>(this->shared_from_this(), GetModel());
  }*/

  ModelRef model_;
  ViewRef view_;
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
