#ifndef POST_WIDGET_H
#define POST_WIDGET_H

#include "post.h"
#include <QtGui/QWidget>
#include <QList>
#include "ui_post_widget.h"

class CommentWidget;

class PostWidget : public QWidget {

    Q_OBJECT

 public:
  PostWidget(Post1* post);
  ~PostWidget();

 public slots:
  void DeletePost();

 protected:
  void mouseReleaseEvent(QMouseEvent *);

 private slots:
  void PostComment();

 private:
  Ui::PostWidgetClass ui;
  Post1* post_;
  QList<CommentWidget*> comments_;
};

#endif // POST_WIDGET_H
