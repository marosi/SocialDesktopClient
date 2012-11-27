/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef COMMENT_ACTIVITY_H_
#define COMMENT_ACTIVITY_H_

#include "sdc/qtgui/activity.h"

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

#endif // COMMENT_ACTIVITY_H_
