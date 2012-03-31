#ifndef BUDDYCLOUD_VIEW_H
#define BUDDYCLOUD_VIEW_H

#include "b_controller.h"
#include "sdc.h"
#include "ui_buddycloud_view.h"
#include <QtGui/QMainWindow>
#include <QList>
#include "boost/shared_ptr.hpp"

class ContactFrameView;
class PostsFrameView;

class BuddycloudView : public QWidget , public sdc::ServiceView {

  Q_OBJECT

 public:
  BuddycloudView(BController* controller, QWidget* parent = 0);
  ~BuddycloudView();

 public slots:
   void ShowConnecting();
   void ShowState(const QString &state);
   void ShowContact(const QString &uid, const QString &name);

   void CreatePostsView(QObject* controller);

 signals:
  void signalPostsViewCreated(PostsFrameView* view);

 private:
  void CreateActions();

  BController* controller_;
  Ui::BuddycloudViewClass ui;
  QList<ContactFrameView*> contacts_;
};

#endif // BUDDYCLOUD_VIEW_H
