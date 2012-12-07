/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef XMPP_COMMANDS_H_
#define XMPP_COMMANDS_H_

#include <QtGui/QWidget>
#include "ui_xmpp_commands.h"

/**
 * DEPRECATED
 */
class XmppCommands : public QWidget
{
    Q_OBJECT

public:
    friend class BView;

    XmppCommands(QWidget *parent = 0);
    ~XmppCommands();

private:
    Ui::XmppCommandsClass ui;
};

#endif // XMPP_COMMANDS_H_
