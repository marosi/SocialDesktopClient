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

INCLUDEPATH += \
	/home/xangel/projects/boost_1_48_0 \
	/home/xangel/projects/swift \
	/home/xangel/projects/swift/3rdParty/LibIDN/src \
	/home/xangel/projects/swift/3rdParty/LibExpat/src \
	../.. \
	../../ui/qtgui \
	../../core/include

QMAKE_CXXFLAGS += \
    -fmessage-length=0 -fPIC \
    -DBOOST_ALL_NO_LIB -DBOOST_SYSTEM_NO_DEPRECATED -DBOOST_FILESYSTEM_VERSION=2 -DIDNA_STATIC -DXML_STATIC \
    -std=c++0x

DEFINES += BOOST_TT_HAS_OPERATOR_HPP_INCLUDED # because of parse error in BOOST_JOIN macro in moc files
 
LIBS += \
    -L/home/xangel/projects/swift/Swiften \
    -L/home/xangel/projects/swift/3rdParty/Boost \
    -L/home/xangel/projects/swift/3rdParty/LibIDN \
    -L/home/xangel/projects/swift/3rdParty/Expat \
    \
    -lSwiften -lSwiften_Boost -lSwiften_IDN -lSwiften_Expat \
    -lz -lssl -lcrypto -lresolv -lpthread -ldl -lm -lc -lstdc++
    

OTHER_FILES += \
    buddycloud.pro.user

