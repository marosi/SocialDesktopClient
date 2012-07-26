TEMPLATE = lib
TARGET = buddycloud
QT += core \
    gui
HEADERS += \
    pubsub_serializer.h \
    pubsub_requests.h \
    pubsub_parser.h \
    post.h \
    post_widget.h \
    xmpp_commands.h \
    buddycloud_service.h \
    channel.h \
    channel_widget.h \
    ui_new_post_widget.h \
    bc_presenter.h \
    bc_new_contact_widget.h \
    bc_new_account_widget.h \
    bc_model.h \
    bc_contact.h \
    bc_contact_widget.h \
    pubsub_parser_factories.h \
    payloads/pubsub_node.h \
    payloads/pubsub.h \
    payloads/items.h \
    payloads/atom.h \
    comment_widget.h \
    abstract_model.h \
    abstract_presenter.h \
    avatar.h \
    filesystem_storages.h \
    avatar_filesystem_storage.h \
    bc_account.h \
    post_activity.h \
    comment_activity.h
SOURCES += \
    pubsub_serializer.cc \
    post.cc \
    post_widget.cpp \
    xmpp_commands.cpp \
    buddycloud_service.cc \
    main.cc \
    channel.cc \
    channel_widget.cpp \
    bc_presenter.cc \
    bc_model.cc \
    bc_contact.cc \
    pubsub_parser.cc \
    comment_widget.cpp \
    bc_contact_widget.cc \
    filesystem_storages.cpp \
    avatar_filesystem_storage.cpp \
    pubsub_requests.cc \
    post_activity.cpp \
    comment_activity.cpp
FORMS += \
    post_widget.ui \
    new_post_widget.ui \
    xmpp_commands.ui \
    bc_new_contact_widget.ui \
    bc_new_account_widget.ui \
    comment_widget.ui
RESOURCES += 

include(swiften.pro)

INCLUDEPATH += \
  3rdparty/swift \
  ../.. \ # path to SDC base dir
  ../../sdc/core \
  ../../sdc/qtgui

QMAKE_CXXFLAGS += \
    -fmessage-length=0 -fPIC \
    -std=c++0x

DEFINES += \
  LOG_PRODUCER=Buddycloud \
  BOOST_TT_HAS_OPERATOR_HPP_INCLUDED # because of parse error in BOOST_JOIN macro in moc files
