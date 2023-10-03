#!/bin/bash

# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2023 Rene Wolf

source source-common.sh
source source-pi.sh

function tee_build_log
{
	tee -a "$FILE_BUILD_LOG"
}

function gen_build_info_txt
{
	echo "CI_PROJECT_URL: $CI_PROJECT_URL
CI_PIPELINE_URL: $CI_PIPELINE_URL
CI_JOB_URL: $CI_JOB_URL
SEMVER: $SEMVER
COMMIT: $CI_COMMIT_SHA
ISO_8601_DATE: $(date --iso-8601=seconds)" > "$1"
}

function gen_build_info_h
{
	if [[ ! -f "$1" ]] ; then
		die "Can't find '$1', bailing out"
	fi

	if ! running_on_ci ; then
		echo "Not running on CI, will not generate '$1'"
		return 0
	fi

	# TODO this will fail for versions above 9
	if [[ $V_MAJOR -gt 9 ]] || [[ $V_MINOR -gt 9 ]] ; then die "Can't generate BCD for $V_MAJOR $V_MINOR" ; fi
	local usb_bcd_version="$(printf "0x%02x%02x" $V_MAJOR $V_MINOR)"

	echo "
#ifndef _BUILD_INFO_H
#define _BUILD_INFO_H

#define NFO_SEMVER_STR \"$SEMVER\"
#define NFO_GIT_SHA \"$CI_COMMIT_SHA\"
#define NFO_SEMVER_USB_DEV_BCD $usb_bcd_version

#endif
" > "$1"

	echo "<build-info-h>"
	cat "$1"
	echo "</build-info-h>"
}

mkdir -p "$DIR_FIRMWARE_BUILD"
echo "Running on $(uname -a)" | tee_build_log
pico_get_sdk 1.5.1 | tee_build_log
gen_build_info_h "$FILE_BUILD_INFO_H"

cd "$DIR_FIRMWARE" || die "can't cd to '$DIR_FIRMWARE'"

echo "Will run cmake from within '$DIR_FIRMWARE' with PICO_SDK_PATH='$PICO_SDK_PATH'" | tee_build_log
if [[ "$PICO_SDK_PATH" == "" ]] || [[ ! -d "$PICO_SDK_PATH" ]] ; then 
	die "PICO_SDK_PATH='$PICO_SDK_PATH' does not exist"
fi

# https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html
# https://github.com/raspberrypi/pico-sdk/tree/master/src/boards/include/boards
PICO_SDK_PATH="$PICO_SDK_PATH" cmake . | tee_build_log

threads=$(get_compile_threads)
echo "Will compile with $threads threads" | tee_build_log
make -j $threads | tee_build_log

gen_build_info_txt "$FILE_BUILD_INFO_TXT"
