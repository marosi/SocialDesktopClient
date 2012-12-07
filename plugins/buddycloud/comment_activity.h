/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef COMMENT_ACTIVITY_H_
#define COMMENT_ACTIVITY_H_

#include "sdc/qtgui/activity.h"
#include "Swiften/JID/JID.h"
#include <string>

class BcPresenter;
class Comment;

/**
 * Commenting activity widget.
 */
class CommentActivity : public sdc::Activity {
 public:
  CommentActivity(BcPresenter* parent, const Swift::JID &from, const Swift::JID &to);

 protected:
  void mouseDoubleClickEvent(QMouseEvent*);

 private:
  BcPresenter* presenter_;
  Swift::JID from_jid_;
  Swift::JID to_jid_;
};

#endif // COMMENT_ACTIVITY_H_
