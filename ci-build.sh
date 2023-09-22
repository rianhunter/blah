#!/bin/bash

source source-common.sh
source source-pi.sh

pico_get_sdk 1.4.0

cd "$DIR_FIRMWARE" || die "can't cd to '$DIR_FIRMWARE'"

echo "Will run cmake from within '$DIR_FIRMWARE'"
# https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html
# https://github.com/raspberrypi/pico-sdk/tree/master/src/boards/include/boards
PICO_SDK_PATH="$PICO_SDK_PATH" cmake .

threads=$(get_compile_threads)
echo "Will compile with $threads threads"
make -j $threads

echo "CI_PROJECT_URL: $CI_PROJECT_URL
CI_PIPELINE_URL: $CI_PIPELINE_URL
CI_JOB_URL: $CI_JOB_URL
SEMVER: $SEMVER
COMMIT: $CI_COMMIT_SHA
ISO_8601_DATE: $(date --iso-8601=seconds)" > "$DIR_FIRMWARE/build/build-info.txt"
