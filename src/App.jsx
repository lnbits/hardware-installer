import { Show, onMount } from "solid-js";
import { marked } from "marked";
import { connected, esploader, running, term } from "./index";
import { Connector } from "./Connector";
import { Configurator } from "./Configurator";
import { Programmer } from "./Programmer";
import logo from "./logo.svg";


export const App = () => {
  let markdownRef;

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
      <header>
        <h1>Generic Installer</h1>
        <img width="180" src={logo} alt="LNbits" />
      </header>
      <p>In order to install LNbits on your hardware, please follow the instructions below.</p>
      <div ref={markdownRef} id="markdown"></div>
      <Connector />
      <Show when={connected()}>
        <Programmer />
        <Configurator />
      </Show>
      <footer>
        <p>LNbits hardware installer, <a target="_blank" href="https://github.com/lnbits/hardware-installer">github</a></p>
      </footer>
    </div>
  );
};
