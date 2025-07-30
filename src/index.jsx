import { render } from "solid-js/web";
import { createSignal } from "solid-js";
import { App } from "./App";
import { Terminal } from "xterm";
import { FitAddon } from "@xterm/addon-fit";
import "../node_modules/xterm/css/xterm.css";
import "./index.css";
import { elements } from "./config";

export const [connected, setConnected] = createSignal(false);
export const [running, setRunning] = createSignal(false);
export const [esploader, setEsploader] = createSignal(null);
export const [config, setConfig] = createSignal(elements);

// Create terminal and fit addon
export const term = new Terminal({
  scrollback: 1000,
  fontSize: 14,
});
export const fitAddon = new FitAddon();
term.loadAddon(fitAddon);

// Render the app first
render(() => <App />, document.getElementById("root"));

// Wait for DOM to mount, then attach terminal
setTimeout(() => {
  const termEl = document.getElementById("terminal");
  if (termEl) {
    term.open(termEl);
    fitAddon.fit();
    term.write("Welcome to the LNbits hardware installer!");

    // Refit terminal on container resize
    new ResizeObserver(() => fitAddon.fit()).observe(termEl);
  }
}, 0);

// Hot reload
if (import.meta.hot) import.meta.hot.accept(() => window.location.reload());
