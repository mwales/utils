# utils

Small utilities that I (or others) may find helpful.

EasyCrypt:  Super simple to use encryption library, intended for prototyping and
CTF style of work.  But implements AES CBC, AES CTR, and a bunch of checksum
algorithms.  Didn't do any public private style encryption though.

EasyRegex:  Wrapper for the C POXIX regular expression library.  Takes care of
managing the memory used by the library, and then uses std::strings for patterns
and matches.

hashsort:  I sometimes will run the sort application on a md5 hash file which
then sorts them by hash.  This application will resort them back into order
based on filename.

hexdump:  Easy to include and fancy hexadecimal dumping tool.  I'm not wasting
time reinventing this thing anymore just to print out a buffer of data.

# Scripts

fix_khugepaged.sh:  Using VMWare on Ubuntu 14.04 or 16.04, and it starts to get
really slow and khugepaged starts burning mucho CPU, this script will fix.

fix_mouse.sh:  I used to have a problem where something would change my mouse
sensitivity or resolution, and I would have to use this script to reset.  I think
this issue resolved itself when I upgraded to Ubuntu 14.04

graphics:  A series of scripts I use to configure my monitors and NVidia drivers
on my Linux system.  Say bye bye to screen tearing.  Not sure why they don't work
like this stock...

video_metadata_rename.sh:  Script to rename video files based on the date and 
time they were encoded.  Useful for when files lose their filenames (during 
data recovery?)



