#ifndef XMPP_COMMANDS_H
#define XMPP_COMMANDS_H

#include <QtGui/QWidget>
#include "ui_xmpp_commands.h"

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

#endif // XMPP_COMMANDS_H
