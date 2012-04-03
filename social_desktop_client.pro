<`TEMPLATE = app
TARGET = social_desktop_client
QT += core \
    gui
HEADERS += ui/qtgui/mainwindow.h \
    ui/qtgui/settings_dialog.h \
    core/include/qt_settings_controller.h \
    core/include/qt_service_controller.h \
    core/include/core_anchor.h \
    core/include/ui.h \
    core/include/qt_gui.h \
    core/include/commands.h \
    core/include/contact.h \
    core/include/content.h \
    core/include/core_typedefs.h \
    core/include/service_view.h \
    core/include/service_model.h \
    core/include/service_controller.h \
    core/include/qt_controller.h \
    core/include/mvc_typedefs.h \
    core/include/connection.h \
    core/include/qt_view.h \
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
SOURCES += ui/qtgui/settings_dialog.cc \
    core/qt_gui.cc \
    core/ui.cc \
    core/qt_view.cc \
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
FORMS += ui/qtgui/mainwindow.ui \
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
