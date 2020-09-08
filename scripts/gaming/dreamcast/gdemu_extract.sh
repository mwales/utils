#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Usage: $0 image_stuff.7z destination"
	exit 1
fi

NEXT_DIR=NULL


for i in {02..99}
do
	if [ ! -e $2/$i ]; then
		if [ $NEXT_DIR = NULL ]; then
			echo "setting it to $2/$i"
			NEXT_DIR=$2/$i
			NEXT_DIR_NUM=$i
		fi
	fi
done

echo "NEXT_DIR=$NEXT_DIR"

SIMPLE_NAME="`basename -s \.7z \"$1\"`"

echo "Start extraction"
echo "7z x \"$1\" -o$NEXT_DIR"
7z x "$1" -o$NEXT_DIR
echo "Extraction complete"

# Is there a CDI file I should rename?
CDI_COUNT=`ls -l "$NEXT_DIR" | grep -i CDI | wc -l`
GDI_COUNT=`ls -l "$NEXT_DIR" | grep -i GDI | wc -l`

echo "CDI count = $CDI_COUNT and GDI count = $GDI_COUNT"

if [[ $CDI_COUNT -eq 1 ]]; then
	CDI_FILENAME=`ls "$NEXT_DIR" | grep -i CDI`
	echo "It's a CDI file: $CDI_FILENAME"
	mv "$NEXT_DIR/$CDI_FILENAME" $NEXT_DIR/disc.cdi
fi

if [[ $GDI_COUNT -eq 1 ]]; then
	GDI_FILENAME=`ls "$NEXT_DIR" | grep -i GDI`
	echo "It's a GDI file: $GDI_FILENAME"
	mv "$NEXT_DIR/$GDI_FILENAME" $NEXT_DIR/disc.gdi

fi

echo "$NEXT_DIR_NUM = $SIMPLE_NAME" >> $2/index.txt



