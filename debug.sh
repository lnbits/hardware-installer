if [ -z "$1" ]; then
    echo "Usage: ./debug.sh /dev/ttyACM0 esp32"
    exit 1
fi
board="esp32:esp32:ttgo-lora32"
# if tdisplay_s3 use esp32s3 board
if [ "$2" = "tdisplay_s3" ]; then
    board="esp32:esp32:esp32s3"
fi
sh build.sh $2 && \
arduino-cli upload --input-dir build --fqbn $board -p $1 && \
arduino-cli monitor -p $1 -c baudrate=115200
