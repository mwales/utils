# Installing MS core fonts offline

There is an Ubuntu package that will try to download and install the MS fonts
directly from sourceforge.  If you have an online Ubuntu system (but it gets
packages from an Ubuntu mirror), you can install this font package, but it
will try and fail to download the fonts.

# What worked

Run the script in this directory on the internet, download the font files, and
then tar.gz the whole directory and copy to your offline system.

Copy all the font .exe files to /var/lib/update-notifier/package-data-downloader

Then run the following

```
sudo /usr/lib/update-notifier/package-data-downloader
```

This would normally try to download from sourceforge and fail.  But I guess it
checks beforehand which files are here already, and since all your files are
already in the download directory, it confirms the hash, and then moves on with
the rest of the installation.

## Failed method ...

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

