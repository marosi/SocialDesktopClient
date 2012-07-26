#!/bin/sh

# build Swiften library
cd 3rdparty/swift
./scons Swiften
cd ../..

# create swiften.pro file with Swiften build configuration
CFLAGS="QMAKE_CXXFLAGS += "`./3rdparty/swift/Swiften/Config/swiften-config --cflags`
LIBS="LIBS += "`./3rdparty/swift/Swiften/Config/swiften-config --libs`

echo "$CFLAGS
$LIBS" > swiften.pro

# run qmake & make
qmake buddycloud.pro
make