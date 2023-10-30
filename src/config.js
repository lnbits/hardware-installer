export const addressesAndFiles = [
  {
    address: "0x1000",
    fileName: "bootloader.bin",
  },
  {
    address: "0x8000",
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

export const configPath = "elements.json";

export const elements = [
  {
    name: "password",
    value: "ToTheMoon1",
    label: "Password for PoS AP WiFi",
    type: "text",
  },
  {
    name: "ssid",
    value: "",
    label: "WiFi SSID",
    type: "text",
  },
  {
    name: "wifipassword",
    value: "",
    label: "WiFi Password",
    type: "text",
  },
  {
    name: "socket",
    value: "",
    label: "LNURLDEVICE SWITCH OPTION Use LNURLDevices extension in LNbits",
    type: "text",
  },
  {
    name: "wallet",
    value: "",
    label: "THRESHOLD OPTION (use lnurlp extension) wallet ID from LNbits",
    type: "text",
  },
  {
    name: "threshold",
    value: "",
    label: "THRESHOLD OPTION amount to hit to trigger",
    type: "text",
  },
  {
    name: "thresholdPin",
    value: "",
    label: "THRESHOLD OPTION pin to trigger",
    type: "text",
  },
  {
    name: "thresholdTime",
    value: "",
    label: "THRESHOLD OPTION time to turn of for",
    type: "text",
  },
];
