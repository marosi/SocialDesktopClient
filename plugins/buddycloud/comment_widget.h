#ifndef COMMENT_WIDGET_H
#define COMMENT_WIDGET_H

#include "abstract_presenter.h"
#include <QWidget>

class Comment;

namespace Ui {
class CommentWidget;
}

class CommentWidget : public QWidget, public AbstractPresenter {

    Q_OBJECT
    
  public:
    explicit CommentWidget(AbstractPresenter* presenter, Comment* comment);
    ~CommentWidget();
    
  private:
    Ui::CommentWidget *ui;
    Comment* comment_;
};

#endif // COMMENT_WIDGET_H
