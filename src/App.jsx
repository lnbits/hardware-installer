import { Show, onMount } from "solid-js";
import { marked } from "marked";
import { config, setConfig, connected, esploader, running, term } from "./index";
import { Connector } from "./Connector";
import { Configurator } from "./Configurator";
import { Programmer } from "./Programmer";
import logo from "./logo.svg";
import data from "./versions.json";

export const App = () => {
  let markdownRef;
  let configData = "";
  let lineBuffer = "";

  const checkFileRead = (data) => {
    if (lineBuffer.length > 10000) {
      console.warn("Line buffer exceeded 10,000 characters, resetting.");
      lineBuffer = "";
    }
    lineBuffer += new TextDecoder().decode(data);
    if (lineBuffer.includes("/file-done")) {
      for (const line of lineBuffer.split("\n")) {
        if (line.startsWith("/file-send")) {
          configData += line.replace("/file-send ", "") + "\n";
        }
        if (line.startsWith("/file-done")) {
          const jsonData = JSON.parse(configData);
          console.log("Configuration data received", jsonData);
          const cfg = config()
          cfg.forEach((element) => {
            if (element.type !== "heading") {
              jsonData.forEach((jsonElement) => {
                if (jsonElement.name === element.name) {
                  element.value = jsonElement.value || "";
                }
              });
            }
          });
          setConfig(null)
          setConfig(cfg)
          lineBuffer = "";
          configData = "";
        }
      }
    }
  };

  onMount(async () => {
    const response = await fetch("INSTALLER.md");
    const text = await response.text();
    markdownRef.innerHTML = marked(text);

    // serial monitor
    while (true) {
      if (!connected() || running()) {
        await new Promise((resolve) => setTimeout(resolve, 1000));
        continue;
      }
      try {
        const val = await esploader().transport.rawRead(1000);
        // await new Promise((resolve) => setTimeout(resolve, 100));
        checkFileRead(val);
        term.write(val);
      } catch (e) {
        if (e.message !== "Timeout") {
          console.error(e);
          term.writeln(e.message);
        }
        await new Promise((resolve) => setTimeout(resolve, 1000));
      }
    }
  });

  return (
    <div id="app">
      <header>
        <h1>{data.name}</h1>
        <a href="https://lnbits.com" target="_blank">
          <img width="180" src={logo} alt="LNbits" />
        </a>
      </header>
      <ol>
        <li>Connect your device to your computer using a USB cable.</li>
        <li>
          Connect the installer to your device by clicking the "Connect to
          Device" button
        </li>
        <li>
          You may need to press the "boot" button on the ESP32 to start the
          connection
        </li>
        <li>
          Upload the firmware to the device by clicking the "Upload Firmware"
          button
        </li>
        <li>
          Upload configuration to your device settings by clicking the
          "Upload config" button
        </li>
      </ol>
      <div className="box">
        <Show when={!("serial" in navigator)}>
          <h2>Browser does not support WebSerial.</h2>
          <p>WebSerial is only supported in Chromium, Chrome or Edge.</p>
        </Show>
        <Show when={"serial" in navigator}>
          <Connector />
          <Show when={connected()}>
            <Programmer />
            <Configurator />
          </Show>
        </Show>
      </div>
      <div ref={markdownRef} id="markdown"></div>
      <footer>
        <p>
          LNbits hardware installer,{" "}
          <a
            target="_blank"
            href="https://github.com/lnbits/hardware-installer"
          >
            github
          </a>
        </p>
      </footer>
    </div>
  );
};
