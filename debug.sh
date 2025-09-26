if [ -z "$1" ]; then
    echo "Usage: ./debug.sh /dev/ttyACM0 esp32"
    exit 1
fi
board="esp32:esp32:ttgo-lora32"
if [ "$2" = "tdisplay_s3" ]; then
    board="esp32:esp32:lilygo_t_display_s3"
    echo -e "\033[0;33mHOLD BOOT BUTTON AND RESET DEVICE TO UPLOAD!\033[0m"
fi
sh build.sh $2 && \
arduino-cli upload --input-dir build --fqbn $board -p $1 && \
arduino-cli monitor --fqbn $board -p $1 -c baudrate=115200
