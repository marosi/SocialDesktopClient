TEMPLATE = app
TARGET = social_desktop_client
QT += core \
    gui
HEADERS += \
    sdc/core/core_anchor.h \
    sdc/core/ui.h \
    sdc/core/content.h \
    sdc/core/service_model.h \
    sdc/core/event_manager.h \
    sdc/core/abstract_manager.h \
    sdc/core/core.h \
    sdc/core/log.h \
    sdc/core/common.h \
    sdc/core/service.h \
    sdc/core/plugin_manager.h \
    sdc/core/properties.h \
    sdc/core/account.h \
    sdc/core/data_manager.h \
    sdc/qtgui/widget_factory.h \
    sdc/qtgui/settings_dialog.h \
    sdc/qtgui/service_presenter.h \
    sdc/qtgui/qt_view.h \
    sdc/qtgui/qt_service_model.h \
    sdc/qtgui/qt_service.h \
    sdc/qtgui/qt_gui.h \
    sdc/qtgui/new_contact_widget.h \
    sdc/qtgui/new_contact_dialog.h \
    sdc/qtgui/new_account_widget.h \
    sdc/qtgui/new_account_dialog.h \
    sdc/qtgui/main_window.h \
    sdc/qtgui/generic_widget_factory.h \
    sdc/qtgui/content_panel.h \
    sdc/qtgui/contact_widget.h \
    sdc/qtgui/bind.h \
    sdc/qtgui/activity_panel.h \
    sdc/qtgui/activity.h \
    sdc/qtgui/accounts_model.h \
    sdc/qtgui/account_button.h
SOURCES += \
    sdc/core/service_model.cc \
    sdc/core/ui.cc \
    sdc/core/event_manager.cc \
    sdc/core/core.cc \
    sdc/core/log.cc \
    sdc/core/service.cc \
    sdc/core/plugin_manager.cc \
    sdc/core/data_manager.cc \
    main.cpp \
    sdc/qtgui/settings_dialog.cc \
    sdc/qtgui/service_presenter.cc \
    sdc/qtgui/qt_service_model.cc \
    sdc/qtgui/qt_service.cc \
    sdc/qtgui/qt_gui.cc \
    sdc/qtgui/new_contact_dialog.cpp \
    sdc/qtgui/new_account_dialog.cpp \
    sdc/qtgui/main_window.cpp \
    sdc/qtgui/content_panel.cpp \
    sdc/qtgui/contact_widget.cpp \
    sdc/qtgui/activity_panel.cpp \
    sdc/qtgui/activity.cpp \
    sdc/qtgui/accounts_model.cpp \
    sdc/qtgui/account_button.cpp
FORMS += \
    sdc/qtgui/settings_dialog.ui \
    sdc/qtgui/new_contact_dialog.ui \
    sdc/qtgui/new_account_dialog.ui \
    sdc/qtgui/main_window.ui \
    sdc/qtgui/content_panel.ui \
    sdc/qtgui/contact_widget.ui \
    sdc/qtgui/activity.ui \
    sdc/qtgui/account_button.ui
RESOURCES += 
INCLUDEPATH += \
    sdc \
    sdc/core \
    sdc/qtgui \
    /home/xangel/projects/boost_1_48_0
LIBS += -L/home/xangel/projects/boost_1_48_0/stage/lib \
    -lboost_system \
    -lboost_thread \
    -lboost_serialization \
    -lboost_filesystem
QMAKE_LFLAGS += -export-dynamic
QMAKE_CXXFLAGS += -std=c++0x

DEFINES = BOOST_FILESYSTEM_VERSION=3
