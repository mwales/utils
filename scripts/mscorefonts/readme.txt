This is from the following post:
https://lists.debian.org/debian-user/2015/07/msg00193.html

You should then put them in e.g. /usr/local/share/corefonts and then
you can tell the ttf-mscorefonts-installer package where to find
those fonts (as root, before installing ttf-mscorefonts-installer):

echo "ttf-mscorefonts-installer msttcorefonts/dldir string /usr/local/share/corefonts" | debconf-set-selections

(replace the directory by the one you chose)

Then you can just install the ttf-mscorefonts-installer package and
it will take the fonts from that directory instead of downloading
them during postinst.

If postinst of ttf-mscorefonts-installer already failed (like it did
in your case), you can still do that debconf-set-selections line and
just retry the installation of the package (e.g. via
dpkg --configure -pending).

