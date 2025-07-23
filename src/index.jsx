import { render } from "solid-js/web";
import { createSignal } from "solid-js";
import { App } from "./App";
import { Terminal } from "xterm";
import "../node_modules/xterm/css/xterm.css";
import "./index.css";
import { elements } from "./config";

export const [connected, setConnected] = createSignal(false);
export const [running, setRunning] = createSignal(false);

export const [esploader, setEsploader] = createSignal(null);

export const [config, setConfig] = createSignal(elements);


export const term = new Terminal({ cols: 120, rows: 14 });
term.open(document.getElementById("terminal"));
term.write("Welcome to the LNbits hardware installer!");

render(
    () => <App />,
    document.getElementById("root")
);

if (import.meta.hot) import.meta.hot.accept(() => window.location.reload());
