/**
 * @file qt_gui.h
 * @brief TODO:
 *	
 * TODO: Complete detailed description
 *
 * @author Maros Kasinec
 *   @date Mar 31, 2012
 */

#ifndef QT_GUI_H_
#define QT_GUI_H_

#include "ui.h"
#include "mainwindow.h"
#include "mvc_typedefs.h"
#include "qt_service_controller.h"
#include <QApplication>
#include "boost/foreach.hpp"
#include "boost/cast.hpp"

namespace sdc {

class QtGui : public Ui {
 public:
  QtGui(Core* core, int argc, char* argv[]) : Ui(core), app_(argc, argv) {}

  void Init() {
    // Init core models, controllers and views
    Ui::Init();

    //TestControllerInit();
    main_view_ = new MainWindow;

    BOOST_FOREACH (const ServiceController* sc, GetControllers()) {
      const QtServiceController* c = boost::polymorphic_downcast<const QtServiceController*>(sc);
      main_view_->AddServiceWidget(c->GetView());
    }

    main_view_->show();
  }

  int Exec() {
    return app_.exec();
  }

 private:
  QApplication app_;
  MainWindow* main_view_;
};

} /* namespace sdc */

#endif /* QT_GUI_H_ */
