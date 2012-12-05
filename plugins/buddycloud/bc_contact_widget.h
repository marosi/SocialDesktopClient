/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef BC_CONTACT_WIDGET_H_
#define BC_CONTACT_WIDGET_H_

#include "abstract_presenter.h"
#include "sdc/qtgui/contact_widget.h"

class BcContact;
class BcPresenter;
class ChannelPanel;
class QAction;

class BcContactWidget : public sdc::ContactWidget, public AbstractPresenter {

    Q_OBJECT

 public:
  BcContactWidget(BcPresenter* presenter, BcContact* contact);
  ~BcContactWidget();

 protected:
  void mouseDoubleClickEvent(QMouseEvent* event);
  void contextMenuEvent(QContextMenuEvent* event);

 private slots:
  void Unfollow();

 private:
  BcContact* contact_;
  QAction* unfollow_;
};

#endif // BC_CONTACT_WIDGET_H_
