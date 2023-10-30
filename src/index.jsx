import { render } from "solid-js/web";
import { createSignal } from "solid-js";
import { App } from "./App";
import { Terminal } from "xterm";
import "../node_modules/xterm/css/xterm.css";
import "./index.css";

export const [connected, setConnected] = createSignal(false);
export const [running, setRunning] = createSignal(false);

export const [esploader, setEsploader] = createSignal(null);


export const term = new Terminal({ cols: 120, rows: 34 });
term.open(document.getElementById("terminal"));

render(
    () => <App />,
    document.getElementById("root")
);

if (import.meta.hot) import.meta.hot.accept(() => window.location.reload());
