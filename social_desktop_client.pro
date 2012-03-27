TEMPLATE = app
TARGET = social_desktop_client
QT += core \
    gui
HEADERS += core/include/commands.h \
    core/include/contact.h \
    core/include/content.h \
    core/include/core_typedefs.h \
    core/include/service_view.h \
    core/include/service_model.h \
    core/include/service_controller.h \
    core/include/qt_controller.h \
    core/include/main_view.h \
    core/include/mvc_typedefs.h \
    core/include/connection.h \
    core/include/qt_view.h \
    testing/test_model.h \
    testing/test_controller.h \
    core/include/observer.h \
    testing/xmltextedit.h \
    core/include/model.h \
    core/include/view.h \
    core/include/controller.h \
    testing/simple_message.h \
    core/include/data_manager.h \
    testing/testconsole.ui.h \
    testing/testwindow.h \
    core/include/event_manager.h \
    core/include/abstract_manager.h \
    core/include/core.h \
    core/include/log.h \
    core/include/common.h \
    core/include/service.h \
    core/include/config_manager.h \
    core/include/connection_manager.h \
    core/include/plugin_manager.h \
    mainwindow.h
SOURCES += core/qt_view.cc \
    core/connection.cc \
    core/observer.cc \
    core/model.cc \
    testing/test_controller.cc \
    testing/simple_message.cc \
    core/controller.cc \
    core/data_manager.cc \
    testing/testwindow.cpp \
    core/event_manager.cc \
    core/abstract_manager.cc \
    core/core.cc \
    core/log.cc \
    core/service.cc \
    core/config_manager.cc \
    core/connection_manager.cc \
    core/plugin_manager.cc \
    main.cpp \
    mainwindow.cpp
FORMS += testing/testwindow.ui \
    mainwindow.ui
RESOURCES += 
INCLUDEPATH += core/include \ # /home/xangel/projects/Plugins
    /home/xangel/projects/boost_1_48_0 \
    testing
LIBS += -L/home/xangel/projects/boost_1_48_0/stage/lib \ # -L/home/xangel/projects/Plugins \
# -lbuddycloud \
    -lboost_system \
    -lboost_thread
QMAKE_LFLAGS += -export-dynamic
QMAKE_CXXFLAGS += -Wunused
