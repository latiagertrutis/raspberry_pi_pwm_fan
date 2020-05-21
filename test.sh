#!/bin/bash

MIN=0
MAX=120000
STEP=5000
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
        sleep 1
    done
done
