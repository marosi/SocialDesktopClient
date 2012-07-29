#ifndef POST_WIDGET_H
#define POST_WIDGET_H

#include "abstract_presenter.h"
#include "ui_post_frame.h"
#include <QFrame>
#include <QList>
#include <QMap>

class CommentFrame;
class Post;
class Comment;

class PostFrame : public QFrame, public AbstractPresenter {

    Q_OBJECT

 public:
  PostFrame(AbstractPresenter* presenter, Post* post);
  ~PostFrame();

 public slots:
  void DeletePost();

 protected:
  void mouseReleaseEvent(QMouseEvent *);

 private slots:
  void PostComment();
  void ShowCommentInOrder(Comment* comment);

 private:
  Ui::PostFrame ui;
  Post* post_;
  QList<Comment*> comments_order_;
  QMap<Comment*, CommentFrame*> comments_;
};

#endif // POST_WIDGET_H
