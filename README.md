
SocialDesktopClient (unstable)
==============================

SocialDesktopClient is a free and open desktop client for social media.

Development is currently focused on [buddycloud channel protocol](https://buddycloud.org/wiki/Main_Page).  
Entire XMPP handling for protocol is done by [Swiften XMPP Library](http://swift.im/swiften).

<b>IMPORTANT</b>: Client is not stable and buddycloud protocol implementation is not finished yet.

To get an image, this is how it looks like so far ...

![screenshot](http://i.imgur.com/9hTTm.png)

## Building

Here are the steps to build and run <b>SocialDesktopClient</b> and <b>buddycloud plugin</b>. Tested on Ubuntu 11.10 (oneiric).

### Requirements

In this early stage SocialDesktopClient requires

* <b>GCC 4.5</b> or later
* <b>Boost 1.46.0</b> or later

### Build steps

1. Install following dev packages

        apt-get install build-essential libboost-all-dev qt4-dev-tools openssl
        
   If above versions of GCC and Boost are not met after installation you can possibly follow steps
   described in [Issue#1](https://github.com/maros-/SocialDesktopClient/issues/1https://github.com/maros-/SocialDesktopClient/issues/1).
   TODO: Bundle Boost within client.
    
2. Clone repo.

        git clone git://github.com/maros-/SocialDesktopClient.git

3. Build SocialDesktopClient

        cd SocialDesktopClient  
        ./build.sh

4. Build buddycloud plugin

        cd plugins/buddycloud  
        ./build.sh
        
5. Run program

        cd ../..
        ./social_desktop_client

## Contact
   
[github](https://github.com/maros-)
   
## Licensing

SocialDesktopClient is available under the terms of GNU General Public License, version 3. See more in COPYING.