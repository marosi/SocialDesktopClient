TEMPLATE = app
TARGET = social_desktop_client
QT += core \
    gui
HEADERS += core/include/controller.h \
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
SOURCES += testing/simple_message.cc \
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
    /home/xangel/boost_1_47_0
LIBS += -L/home/xangel/boost_1_47_0/stage/lib \ # -L/home/xangel/projects/Plugins \
# -lbuddycloud \
    -lboost_system \
    -lboost_thread
QMAKE_LFLAGS += -Wl \
    -export-dynamic
