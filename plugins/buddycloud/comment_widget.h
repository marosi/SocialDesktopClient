#ifndef COMMENT_WIDGET_H
#define COMMENT_WIDGET_H

#include <QWidget>

class Comment;

namespace Ui {
class CommentWidget;
}

class CommentWidget : public QWidget
{
    Q_OBJECT
    
  public:
    explicit CommentWidget(Comment* comment);
    ~CommentWidget();
    
  private:
    Ui::CommentWidget *ui;
    Comment* comment_;
};

#endif // COMMENT_WIDGET_H
