#ifndef BUDDYCLOUD_VIEW_H
#define BUDDYCLOUD_VIEW_H

#include "b_controller.h"
#include "sdc.h"
#include "ui_buddycloud_view.h"
#include <QtGui/QMainWindow>
#include <QList>
#include "boost/shared_ptr.hpp"

class ContactFrameView;
class PostFrameView;

class BuddycloudView : public QMainWindow , public sdc::ServiceView {

  Q_OBJECT

 public:
  BuddycloudView(BController* controller, QWidget* parent = 0);
  ~BuddycloudView();

 public slots:
   void ShowConnecting();
   void ShowState(const QString &state);
   void ShowContact(const QString &uid, const QString &name);
   void ShowPost(const QString &author, const QString &content);

 private:
  void CreateActions();

  BController* controller_;
  Ui::BuddycloudViewClass ui;
  QList<ContactFrameView*> contacts_;
  QList<PostFrameView*> posts_;
};

#endif // BUDDYCLOUD_VIEW_H
