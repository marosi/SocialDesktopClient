#ifndef BUDDYCLOUD_VIEW_H
#define BUDDYCLOUD_VIEW_H

#include <QtGui/QMainWindow>
#include "ui_buddycloud_view.h"
#include "b_controller.h"
#include "sdc.h"
#include "boost/shared_ptr.hpp"

class BuddycloudView : public QMainWindow , public sdc::ServiceView {

  Q_OBJECT

 public:
  BuddycloudView(BController* controller, QWidget *parent = 0);
  ~BuddycloudView();

 public slots:
   void ShowConnecting();
   void ShowState(const QString &state);

 private:
  void CreateActions();

  BController* controller_;
  Ui::BuddycloudViewClass ui;
};

#endif // BUDDYCLOUD_VIEW_H
