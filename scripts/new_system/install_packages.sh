#!/bin/bash

sudo apt-get install android-sdk-platform-tools-common android-tools-adb arandr asciidoc audacity avidemux-qt -y
sudo apt-get install binutils-dev binwalk blender build-essential calibre clang cmake cpio curl -y
sudo apt-get install dolphin-emu dos2unix dosbox doxygen-gui dvdauthor easytag exfat-fuse exfat-utils -y
sudo apt-get install fakeroot fceux fdisk ffmpeg flac ftp fslint -y
sudo apt-get install g++ gcc-arm-linux-gnueabi gdb-multiarch gdbserver geany gdisk gedit gens gftp ghex gimp git gitk gnuradio gparted graphviz guake gwenview -y
sudo apt-get install handbrake hdparm hexchat higan htop hugin -y
sudo apt-get install i2c-tools ifupdown imagemagick inkscape ioquake3 iotop ipython jstest-gtk -y
sudo apt-get install k4dirstat k3b kdenlive kompare -y
sudo apt-get install lynx libreoffice-common mame meld mplayer -y
sudo apt-get install nano nasm netcat-traditional netcat-openbsd net-tools nfs-common -y
sudo apt-get install nmap okular openscad openshot openssh-server -y
sudo apt-get install p7zip putty pavucontrol pcsxr picocom pyqt5-dev -y
sudo apt-get install qdirstat qemu-system qemu-user qemu-utils qt5-default qt5-doc qt5-qmake qtbase5-dev-tools qtcreator -y
sudo apt-get install rar remmina rsync smartmon-tools smplayer socat sqlitebrowser sound-juicer squashfs-tools steam supertuxkart synaptic -y
sudo apt-get install telnet tmux transmission umbrello usb-creator-gtk vice vim valgrind virtualbox-qt vlc vokoscreen -y
sudo apt-get install wget wine-stable winetricks wireshark xfce4-sensors-plugin youtube-dl zsnes yabause -y


# Things that used to be in there that aren't anymore
# ckermit
# oracle-java8-installer 
# photoprint
# plexmediaserver
# unetbootin

# Need to update this script to just install 1 package at a time, and track the failures.  It will
# miss a package and then install any of the other packages on the line
