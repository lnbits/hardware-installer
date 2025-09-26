#!/bin/sh
if [ -z "$1" ]; then
    echo "Usage: sh build.sh <device>"
    echo "esp32 is the default device"
    exit 1
fi
device_name=$(echo $1 | tr '[:lower:]' '[:upper:]')
tft_config_file="tft_config_$1.txt"
if [ -f "$tft_config_file" ]; then
    user_tft_config=$(cat $tft_config_file | tr '\n' ' ' | sed -e "s/\ /\ -D/g" -e "s/-D$//")
    tft_font="-DLOAD_GLCD=1 -DLOAD_FONT2=1 -DLOAD_FONT4=1 -DLOAD_FONT6=1 -DLOAD_FONT7=1 -DLOAD_FONT8=1 -DLOAD_GFXFF=1 -DSMOOTH_FONT=1"
    tft_config=" -DTFT=1 -DUSER_SETUP_LOADED=1 -D${user_tft_config} ${tft_font}"
fi

# works for tdisplay and esp32 devmodule
board="esp32:esp32:ttgo-lora32"
flags="build.extra_flags.esp32=-DARDUINO_USB_CDC_ON_BOOT=0"

# if tdisplay_s3 use esp32s3 board
if [ "$1" = "tdisplay_s3" ]; then
    board="esp32:esp32:lilygo_t_display_s3"
    flags="build.extra_flags.esp32s3=-DARDUINO_USB_MODE={build.usb_mode} -DARDUINO_USB_CDC_ON_BOOT={build.cdc_on_boot} -DARDUINO_USB_MSC_ON_BOOT={build.msc_on_boot} -DARDUINO_USB_DFU_ON_BOOT={build.dfu_on_boot}"
fi

extra_flags="${flags} -D${device_name}=1${tft_config}"
echo "Extra build flags: ${extra_flags}"

arduino-cli compile \
    --build-property "build.partitions=min_spiffs" \
    --build-property "upload.maximum_size=1966080" \
    --build-property "${extra_flags}" \
    --build-path build \
    --fqbn $board genericInstaller
