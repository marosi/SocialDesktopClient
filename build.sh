#!/bin/sh

# build boost thread lib
#cd 3rdparty/boost_1_48_0
#./bootstrap.sh --with-libraries=thread
#./b2
#cd ../..


# run qmake & make
qmake social_desktop_client.pro
make