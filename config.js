export const addressesAndFiles = [
  {
    address: "0x1000",
    fileName: "genericInstaller.ino.bootloader.bin",
  },
  {
    address: "0x8000",
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

export const configPath = "elements.json";

export const elements = [
  {
    type: "heading",
    label: "WiFi Settings",
  },
  {
    name: "config_led_pin",
    value: "",
    label: "LED pin",
    type: "text",
  },
  {
    name: "config_ssid",
    value: "",
    label: "WiFi SSID",
    type: "text",
  },
  {
    name: "config_password",
    value: "",
    label: "WiFi password",
    type: "text",
  }
];
