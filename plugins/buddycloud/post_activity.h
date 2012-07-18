#ifndef POST_ACTIVITY_H
#define POST_ACTIVITY_H

#include "activity.h"

class BcPresenter;
class Post1;

class PostActivity : public sdc::Activity {
 public:
  PostActivity(BcPresenter* parent, const Post1* post);

 protected:
  void mouseDoubleClickEvent(QMouseEvent*);

 private:
  BcPresenter* presenter_;
  const Post1* post_;
};

#endif // POST_ACTIVITY_H
