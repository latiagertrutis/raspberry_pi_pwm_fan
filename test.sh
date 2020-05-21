#!/bin/bash

MIN=47000
MAX=80000
STEP=500
TEMP_FILE="pwm_test.txt"

trap stop SIGINT

stop() {
    echo "Script stopped."
    rm "$TEMP_FILE"
    exit
}

while :
do
    for i in $(seq $MIN $STEP $MAX);
    do
        echo "$i" > "$TEMP_FILE"
        sleep 3
    done
done
