#ifndef POST_WIDGET_H
#define POST_WIDGET_H

#include "content/channel.h"
#include "content/post.h"
#include "sdc.h"
#include <QtGui/QWidget>
#include "ui_post_widget.h"

class PostWidget : public QWidget, public sdc::QtView {

    Q_OBJECT

 public:
  PostWidget(QWidget *parent, Post1::Ref post);
  ~PostWidget();

 public slots:
  void DeletePost();

 signals:
  void remove();

 private:
  void OnRemove() {
    emit remove();
  }

  Ui::PostWidgetClass ui;
  Post1::Ref post_;
};

#endif // POST_WIDGET_H
