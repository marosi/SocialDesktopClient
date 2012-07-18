TEMPLATE = app
TARGET = social_desktop_client
QT += core \
    gui
HEADERS += ui/qtgui/bind.h \
    ui/qtgui/main_window.h \
    ui/qtgui/service_presenter.h \
    ui/qtgui/new_contact_widget.h \
    ui/qtgui/new_contact_dialog.h \
    ui/qtgui/generic_widget_factory.h \
    ui/qtgui/widget_factory.h \
    ui/qtgui/content_panel.h \
    ui/qtgui/contact_widget.h \
    ui/qtgui/account_button.h \
    ui/qtgui/new_account_widget.h \
    ui/qtgui/qt_service.h \
    ui/qtgui/new_account_dialog.h \
    ui/qtgui/qt_service_model.h \
    core/include/account_data.h \
    ui/qtgui/qt_view.h \
    ui/qtgui/qt_gui.h \
    ui/qtgui/settings_dialog.h \
    core/include/core_anchor.h \
    core/include/ui.h \
    core/include/content.h \
    core/include/service_model.h \
    core/include/connection.h \
    core/include/event_manager.h \
    core/include/abstract_manager.h \
    core/include/core.h \
    core/include/log.h \
    core/include/common.h \
    core/include/service.h \
    core/include/config_manager.h \
    core/include/connection_manager.h \
    core/include/plugin_manager.h \
    core/include/properties.h \
    ui/qtgui/accounts_model.h
SOURCES += ui/qtgui/service_presenter.cc \
    ui/qtgui/main_window.cpp \
    ui/qtgui/new_contact_dialog.cpp \
    ui/qtgui/content_panel.cpp \
    ui/qtgui/contact_widget.cpp \
    ui/qtgui/qt_service_model.cc \
    ui/qtgui/qt_service.cc \
    ui/qtgui/account_button.cpp \
    core/service_model.cc \
    ui/qtgui/new_account_dialog.cpp \
    ui/qtgui/qt_gui.cc \
    ui/qtgui/settings_dialog.cc \
    core/ui.cc \
    core/connection.cc \
    core/event_manager.cc \
    core/core.cc \
    core/log.cc \
    core/service.cc \
    core/config_manager.cc \
    core/connection_manager.cc \
    core/plugin_manager.cc \
    main.cpp \
    ui/qtgui/accounts_model.cpp
FORMS += ui/qtgui/main_window.ui \
    ui/qtgui/new_contact_dialog.ui \
    ui/qtgui/content_panel.ui \
    ui/qtgui/contact_widget.ui \
    ui/qtgui/account_button.ui \
    ui/qtgui/new_account_dialog.ui \
    ui/qtgui/settings_dialog.ui \
    ui/qtgui/contacts_group.ui
RESOURCES += 
INCLUDEPATH += core/include \
    ui/qtgui \
    /home/xangel/projects/boost_1_48_0
LIBS += -L/home/xangel/projects/boost_1_48_0/stage/lib \
    -lboost_system \
    -lboost_thread \
    -lboost_serialization \
    -lboost_filesystem
QMAKE_LFLAGS += -export-dynamic
QMAKE_CXXFLAGS += -std=c++0x

DEFINES = BOOST_FILESYSTEM_VERSION=3
