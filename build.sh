#!/bin/sh
project_name="genericInstaller"

if [ -z "$1" ]; then
    echo "Usage: sh build.sh <device>"
    echo "esp32 is the default device"
    exit 1
fi

device_name=$(echo $1 | tr '[:lower:]' '[:upper:]')
config_file="./devices/$1.txt"

if [ ! -f "$config_file" ]; then
    echo "Config file $config_file does not exist."
    exit 1
fi

device_config=$(cat $config_file | tr '\n' ' ' | sed -e "s/\ /\ -D/g" -e "s/-D$//")

# works for tdisplay and esp32 devmodule
board="esp32:esp32:ttgo-lora32"
flags="build.extra_flags.esp32="

# if tdisplay_s3 use esp32s3 board
if [ "$1" = "tdisplay_s3" ]; then
    board="esp32:esp32:lilygo_t_display_s3"
    flags="build.extra_flags.esp32s3="
fi

if [ "$1" = "guition_s3" ]; then
    board="esp32:esp32:lilygo_t_display_s3"
    flags="build.extra_flags.esp32s3="
fi

extra_flags="${flags}-D${device_config}"
echo "Extra build flags: ${extra_flags}"

arduino-cli compile \
    --build-property "build.partitions=min_spiffs" \
    --build-property "upload.maximum_size=1966080" \
    --build-property "${extra_flags}" \
    --build-path build \
    --fqbn $board $project_name
