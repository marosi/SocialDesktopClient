#ifndef POST_WIDGET_H
#define POST_WIDGET_H

#include "abstract_presenter.h"
#include "ui_post_widget.h"
#include <QtGui/QWidget>
#include <QList>
#include <QMap>

class CommentWidget;
class Post;
class Comment;

class PostWidget : public QWidget, public AbstractPresenter {

    Q_OBJECT

 public:
  PostWidget(AbstractPresenter* presenter, Post* post);
  ~PostWidget();

 public slots:
  void DeletePost();

 protected:
  void mouseReleaseEvent(QMouseEvent *);

 private slots:
  void PostComment();
  void ShowCommentInOrder(Comment* comment);

 private:
  Ui::PostWidgetClass ui;
  Post* post_;
  QList<Comment*> comments_order_;
  QMap<Comment*, CommentWidget*> comments_;
};

#endif // POST_WIDGET_H
