#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage $0 filename"
	echo "It changes the filename from test.txt to reallylonghash_test.txt"
	echo "You supplied $# args like an idiot"
	exit 1
fi

HASH=`md5sum $1 | cut -d' ' -f1`

mv $1 ${HASH}_$1

