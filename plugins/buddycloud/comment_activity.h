#ifndef COMMENT_ACTIVITY_H
#define COMMENT_ACTIVITY_H

#include "activity.h"

class BcPresenter;
class Comment;

class CommentActivity : public sdc::Activity {
 public:
  CommentActivity(BcPresenter* parent, const Comment* comment);

 protected:
  void mouseDoubleClickEvent(QMouseEvent*);

 private:
  BcPresenter* presenter_;
  const Comment* comment_;
};

#endif // COMMENT_ACTIVITY_H
