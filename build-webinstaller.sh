#!/bin/sh
PROJECT_NAME=genericInstaller
RELEASE=https://github.com/lnbits/hardware-installer/releases/download
INSTALLER_REPO=https://github.com/lnbits/hardware-installer
INSTALLER_PATH=./hardware-installer/public/firmware

git clone $INSTALLER_REPO

cp INSTALLER.md ./hardware-installer/public/INSTALLER.md
cp versions.json ./hardware-installer/src/versions.json
cp config.js ./hardware-installer/src/config.js

sed -i "s/%title%/$PROJECT_NAME/g" ./hardware-installer/index.html

mkdir -p $INSTALLER_PATH
for device in $(jq -r '.devices[]' ./hardware-installer/src/versions.json); do
    for version in $(jq -r '.versions[]' ./hardware-installer/src/versions.json); do
        mkdir -p $INSTALLER_PATH/$device/$version
        wget $RELEASE/$version/$PROJECT_NAME-$device.ino.bin
        wget $RELEASE/$version/$PROJECT_NAME-$device.ino.partitions.bin
        wget $RELEASE/$version/$PROJECT_NAME-$device.ino.bootloader.bin
        mv $PROJECT_NAME-$device.ino.bin $INSTALLER_PATH/$device/$version/main.bin
        mv $PROJECT_NAME-$device.ino.partitions.bin $INSTALLER_PATH/$device/$version/partitions.bin
        mv $PROJECT_NAME-$device.ino.bootloader.bin $INSTALLER_PATH/$device/$version/bootloader.bin
    done
done
