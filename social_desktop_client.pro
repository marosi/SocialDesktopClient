<`TEMPLATE = app
TARGET = social_desktop_client
QT += core \
    gui
HEADERS += ui/qtgui/contact_widget.h \
    ui/qtgui/account_button.h \
    ui/qtgui/new_account_widget.h \
    ui/qtgui/qt_service.h \
    ui/qtgui/new_account_dialog.h \
    ui/qtgui/qt_service_model.h \
    core/include/account_data.h \
    ui/qtgui/settings_dialog_controller.h \
    ui/qtgui/qt_view.h \
    ui/qtgui/qt_gui.h \
    ui/qtgui/mainwindow.h \
    ui/qtgui/settings_dialog.h \
    core/include/core_anchor.h \
    core/include/ui.h \
    core/include/commands.h \
    core/include/content.h \
    core/include/core_typedefs.h \
    core/include/service_view.h \
    core/include/service_model.h \
    core/include/service_controller.h \
    core/include/mvc_typedefs.h \
    core/include/connection.h \
    core/include/observer.h \
    core/include/model.h \
    core/include/view.h \
    core/include/controller.h \
    core/include/data_manager.h \
    core/include/event_manager.h \
    core/include/abstract_manager.h \
    core/include/core.h \
    core/include/log.h \
    core/include/common.h \
    core/include/service.h \
    core/include/config_manager.h \
    core/include/connection_manager.h \
    core/include/plugin_manager.h
SOURCES += ui/qtgui/contact_widget.cpp \
    ui/qtgui/qt_service_model.cc \
    ui/qtgui/qt_service.cc \
    ui/qtgui/account_button.cpp \
    core/service_model.cc \
    ui/qtgui/new_account_dialog.cpp \
    ui/qtgui/qt_gui.cc \
    ui/qtgui/qt_view.cc \
    ui/qtgui/settings_dialog.cc \
    core/ui.cc \
    core/connection.cc \
    core/observer.cc \
    core/model.cc \
    core/controller.cc \
    core/event_manager.cc \
    core/abstract_manager.cc \
    core/core.cc \
    core/log.cc \
    core/service.cc \
    core/config_manager.cc \
    core/connection_manager.cc \
    core/plugin_manager.cc \
    main.cpp
FORMS += ui/qtgui/contact_widget.ui \
    ui/qtgui/account_button.ui \
    ui/qtgui/new_account_dialog.ui \
    ui/qtgui/mainwindow.ui \
    ui/qtgui/settings_dialog.ui
RESOURCES += 
INCLUDEPATH += core/include \
    ui/qtgui \ # /home/xangel/projects/Plugins \
    /home/xangel/projects/boost_1_48_0

# testing
LIBS += -L/home/xangel/projects/boost_1_48_0/stage/lib \ # -L/home/xangel/projects/Plugins \
# -lbuddycloud \
    -lboost_system \
    -lboost_thread \
    -lboost_serialization
QMAKE_LFLAGS += -export-dynamic
QMAKE_CXXFLAGS += -Wunused
