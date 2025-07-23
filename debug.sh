if [ -z "$1" ]; then
    echo "Usage: ./debug.sh /dev/ttyACM0"
    exit 1
fi
arduino-cli compile \
    --build-property "build.partitions=min_spiffs" \
    --build-property "upload.maximum_size=1966080" \
    --build-path build --fqbn esp32:esp32:ttgo-lora32 genericInstaller && \
arduino-cli upload --input-dir build --fqbn esp32:esp32:ttgo-lora32 -p $1 && \
arduino-cli monitor -p $1 -c baudrate=115200
