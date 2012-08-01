
SocialDesktopClient (unstable)
==============================

SocialDesktopClient is a free and open desktop client for social media.

Development is currently focused on [buddycloud channel protocol](https://buddycloud.org/wiki/Main_Page).

<b>IMPORTANT</b>: Client is not stable and buddycloud protocol implementation is not finished yet.

This is how it looks like so far ...

![screenshot](http://i.imgur.com/9hTTm.png)

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