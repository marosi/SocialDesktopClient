
SocialDesktopClient (unstable)
==============================

SocialDesktopClient is a free and open desktop client for social media.

Development is currently focused on [buddycloud channel protocol](https://buddycloud.org/wiki/Main_Page).

IMPORTANT: Client is not stable yet, and buddycloud protocol implementation is not finished yet.

## Building

Here are the steps to build <b>SocialDesktopClient</b> and <b>buddycloud plugin</b>. Tested on Ubuntu 11.10 (oneiric).

1. Install dev tools 

    apt-get install build-essential libboost-all-dev qt4-dev-tools openssl
    
2. Clone repo.

    git clone git://github.com/maros-/SocialDesktopClient.git

3. Build SocialDesktopClient

    cd SocialDesktopClient
    ./build.sh

4. Build buddycloud plugin

    cd plugins/buddycloud
    ./build.sh

## Contact
   
[github](https://github.com/maros-)
   
## Licensing

SocialDesktopClient is available under the terms of GNU General Public License, version 3. See more in COPYING.