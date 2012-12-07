/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef POST_WIDGET_H_
#define POST_WIDGET_H_

#include "abstract_presenter.h"
#include "ui_post_frame.h"
#include <QFrame>
#include <QList>
#include <QMap>

class CommentFrame;
class Post;
class Comment;

/**
 * Channel topic post widget.
 */
class PostFrame : public QFrame, public AbstractPresenter {

    Q_OBJECT

 public:
  PostFrame(AbstractPresenter* presenter, Post* post);
  ~PostFrame();

 public slots:
  void DeletePost();

 protected:
  void mouseReleaseEvent(QMouseEvent *);
  void enterEvent(QEvent *);
  void leaveEvent(QEvent *);

 private slots:
  void PostComment();
  void ShowCommentInOrder(Comment* comment);

 private:
  Ui::PostFrame ui;
  Post* post_;
  QList<Comment*> comments_order_;
  QMap<Comment*, CommentFrame*> comments_;
};

#endif // POST_WIDGET_H_
