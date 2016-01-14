#!/bin/bash

INPUT_ID=$(xinput --list | grep Logitech | grep -o "id=[[:digit:]]*" | grep -o "[[:digit:]]*")
echo "Found ID $INPUT_ID"
xinput --set-prop $INPUT_ID 278 2.5


