/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef COMMENT_WIDGET_H_
#define COMMENT_WIDGET_H_

#include "abstract_presenter.h"
#include <QFrame>

class Comment;

namespace Ui {
class CommentFrame;
}

/**
 * Channel comment widget.
 */
class CommentFrame : public QFrame, public AbstractPresenter {

    Q_OBJECT
    
  public:
    explicit CommentFrame(AbstractPresenter* presenter, Comment* comment);
    ~CommentFrame();
    
  private:
    Ui::CommentFrame *ui;
    Comment* comment_;
};

#endif // COMMENT_WIDGET_H_
