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
    type: "ACInput",
    value: "ToTheMoon1",
    label: "Password for PoS AP WiFi",
    pattern: "",
    placeholder: "",
    style: "",
    apply: "text",
  },
  {
    name: "ssid",
    type: "ACInput",
    value: "",
    label: "WiFi SSID",
    pattern: "",
    placeholder: "",
    style: "",
    apply: "text",
  },
  {
    name: "wifipassword",
    type: "ACInput",
    value: "",
    label: "WiFi Password",
    pattern: "",
    placeholder: "",
    style: "",
    apply: "text",
  },
  {
    name: "socket",
    type: "ACInput",
    value: "",
    label: "LNURLDEVICE SWITCH OPTION Use LNURLDevices extension in LNbits",
    pattern: "",
    placeholder: "",
    style: "",
    apply: "text",
  },
  {
    name: "wallet",
    type: "ACInput",
    value: "",
    label:
      "THRESHOLD OPTION (use lnurlp extension to make an lnurlp) wallet ID from LNbits",
    pattern: "",
    placeholder: "",
    style: "",
    apply: "text",
  },
  {
    name: "threshold",
    type: "ACInput",
    value: "",
    label: "THRESHOLD OPTION amount to hit to trigger",
    pattern: "",
    placeholder: "",
    style: "",
    apply: "text",
  },
  {
    name: "thresholdPin",
    type: "ACInput",
    value: "",
    label: "THRESHOLD OPTION pin to trigger",
    pattern: "",
    placeholder: "",
    style: "",
    apply: "text",
  },
  {
    name: "thresholdTime",
    type: "ACInput",
    value: "",
    label: "THRESHOLD OPTION time to turn of for",
    pattern: "",
    placeholder: "",
    style: "",
    apply: "text",
  },
];
