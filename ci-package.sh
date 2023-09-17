#!/bin/bash

source source-common.sh
source source-pi.sh

cd "$DIR_FIRMWARE" || die "can't cd to '$DIR_FIRMWARE'"

output_name="cxadc-clock-generator-audio-adc-firmware-${SEMVER}"
output_dir="$DIR_FIRMWARE/$output_name"
mkdir -p "$output_dir"


cmake_exe_name=firmware
mv_or_die "$DIR_FIRMWARE/src/${cmake_exe_name}.elf" "$output_dir/firmware-${SEMVER}.elf"
mv_or_die "$DIR_FIRMWARE/src/${cmake_exe_name}.uf2" "$output_dir/firmware-${SEMVER}.uf2"
cp_or_die "$DIR_FIRMWARE/src/build-info.txt" "$output_dir/"
cp_or_die "$DIR_FIRMWARE/LICENSE" "$output_dir/LICENSE-firmware.txt"
cp_or_die "$DIR_FIRMWARE/pico-sdk/LICENSE.TXT" "$output_dir/LICENSE-pico-sdk.txt"
cp_or_die "$DIR_FIRMWARE/pico-sdk/lib/tinyusb/LICENSE" "$output_dir/LICENSE-tinyusb.txt"

zip -r "${output_name}.zip" "$output_name"
