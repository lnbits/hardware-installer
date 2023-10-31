import { Show, onMount } from "solid-js";
import { marked } from "marked";
import { connected, esploader, running, term } from "./index";
import { Connector } from "./Connector";
import { Configurator } from "./Configurator";
import { Programmer } from "./Programmer";
import logo from "./logo.svg";
// import { SerialMonitor } from "./SerialMonitor";


export const App = () => {
  let markdownRef;

  onMount(async () => {
      const response = await fetch("README.md");
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
          await new Promise((resolve) => setTimeout(resolve, 100));
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
      <button style="margin-top: 12px;" onClick={() => term.clear()}>Clear Terminal</button>
      <h2>Installer</h2>
      <Connector />
      <Show when={connected()}>
        <Programmer />
        <Configurator />
      </Show>
      <div ref={markdownRef} id="markdown"></div>
      <footer>
        <img width="200" src={logo} alt="LNbits" />
        <p>LNbits hardware installer, <a target="_blank" href="https://github.com/lnbits/hardware-installer">github</a></p>
      </footer>
    </div>
  );
};
