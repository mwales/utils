#!/bin/bash

# This script should rename video files based on the date they were encoded.  I needed to write
# this script one day when my Galaxy S5 accidentally erased the contents of my micro SD card.  I
# was able to recover many of the media files stored within, but the names were lost.

# Usage
# ./rename.sh prefix_ filename.mp4

if [ $# -ne 2 ]
then
  echo "This script will rename files based on the date they were encoded"
  echo ""
  echo "Incorrect number of parameters provided"
  echo "Usage is:"
  echo "  $0 prefix_ filename.mp4"
  exit 1
fi

MEDIA_EXISTS=`which mediainfo | wc -l`

if [[ $MEDIA_EXISTS -ne 1 ]]
then
  echo "This application depends on the mediainfo application"
  echo "  sudo apt-get install mediainfo"
  exit 1
fi

ENCODED_DATE=`mediainfo --Inform="Video;%Encoded_Date%" $2`
ENCODED_DATE=`echo $ENCODED_DATE | sed 's/UTC //g' | sed 's/ /-/g' | sed 's/\:/-/g'`

EXTENSION=`echo $2 | sed 's/.*\.//g'`
FINAL_NAME=${1}-${ENCODED_DATE}.${EXTENSION}

# Don't continue if the ENCODED_DATE is blank, or if FINAL_NAME already exists
if [ -e $FINAL_NAME ]
then
  echo "Aborting!  Attempting to rename $1 to $FINAL_NAME, but it already exists!"
  exit 1
fi

if [ "$ENCODED_DATE" = "" ]
then
  echo "Aborting!  Couldn't determine the encoding date!"
  exit 1
fi

echo "Renaming $2 to $FINAL_NAME"

mv $2 $FINAL_NAME

