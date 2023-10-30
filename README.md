# install
```console
npm install
```

# run locally
```console
vite serve
```

# build production
```console
vite build
```

# Configuration

## versions.json
configure firmware, devices and verisons
`src/versions.json`
```json
{
  "devices": [
    "esp32"
  ],
  "versions": [
    "v0.1.0"
  ]
}
```
add firmware accordingly into `src/firmware/esp32/v0.1.0`

## config.js
`src/config.js`

### firmware files
```js
export const addressesAndFiles = [
  {
    address: "0x1000",
    fileName: "bootloader.bin",
  },
  {
    address: "0x9000",
    fileName: "bitcoinSwitch.ino.partitions.bin",
  },
  {
    address: "0xE000",
    fileName: "boot_app0.bin",
  },
  {
    address: "0x10000",
    fileName: "bitcoinSwitch.ino.bin",
  },
];
```

### firmware config (elements.json)
you can define form fields, whichcan be uploaded to the device in serial config mode.
```js
export const elements = [
  {
    name: "password",
    type: "ACInput",
    value: "ToTheMoon1",
    label: "Password for PoS AP WiFi",
    pattern: "",
    placeholder: "Password for PoS AP WiFi",
    style: "",
    apply: "text",
  },
];
