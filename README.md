# Generic Installer

## this serves as an example for the `hardware-installer`

## install

```console
npm install
```

### run locally

```console
npm run serve
```

### build production

```console
npm run build
```

## Configuration

### versions.json

configure firmware, devices and verisons
`src/versions.json`

```json
{
  "devices": ["esp32"],
  "versions": ["v0.1.0"]
}
```

add firmware accordingly into `src/firmware/esp32/v0.0.0`

### config.js

`src/config.js`

#### firmware files

```js
export const addressesAndFiles = [
  {
    address: "0x1000",
    fileName: "genericInstaller.ino.bootloader.bin",
  },
  {
    address: "0x9000",
    fileName: "genericInstaller.ino.partitions.bin",
  },
  {
    address: "0xE000",
    fileName: "boot_app0.bin",
  },
  {
    address: "0x10000",
    fileName: "genericInstaller.ino.bin",
  },
];
```

#### firmware config (elements.json)

you can define form fields, whichcan be uploaded to the device in serial config mode.

```js
export const elements = [
  {
    name: "password",
    value: "ToTheMoon1",
    label: "Password for PoS AP WiFi",
    type: "text",
  },
];
```

## Development

### Helper scripts

install dependencies and libraries

```console
./install.sh
```

compile for device

```console
./build.sh esp32
```

debug (serial port, device type)

```console
./debug.sh /dev/ttyACM0 esp32
```

### arduino-cli

compiling

```console
arduino-cli compile --build-path build --fqbn esp32:esp32:esp32 bitcoinSwitch
```

monitoring

```console
arduino-cli monitor -p /dev/ttyUSB1 -c baudrate=115200
```

uploading

```console
arduino-cli upload --fqbn esp32:esp32:esp32 --input-dir build -p /dev/ttyUSB1
```
