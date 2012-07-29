#ifndef COMMENT_WIDGET_H
#define COMMENT_WIDGET_H

#include "abstract_presenter.h"
#include <QFrame>

class Comment;

namespace Ui {
class CommentFrame;
}

class CommentFrame : public QFrame, public AbstractPresenter {

    Q_OBJECT
    
  public:
    explicit CommentFrame(AbstractPresenter* presenter, Comment* comment);
    ~CommentFrame();
    
  private:
    Ui::CommentFrame *ui;
    Comment* comment_;
};

#endif // COMMENT_WIDGET_H
