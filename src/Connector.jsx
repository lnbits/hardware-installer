import { ESPLoader, Transport } from "esptool-js";
import {
  term,
  running,
  connected,
  setConnected,
  setEsploader,
} from "./index";

export const Connector = () => {
  const connect = async () => {
    try {
      const device = await navigator.serial.requestPort({});
      const transport = new Transport(device);
      const esploader = new ESPLoader({
        transport,
        baudrate: 115200,
        terminal: {
          clean() {
            term.clear();
          },
          writeLine(data) {
            term.writeln(data);
          },
          write(data) {
            term.write(data);
          },
        },
      });
      await esploader.main_fn();
      setEsploader(esploader);
      setConnected(true);
    } catch (e) {
      console.error(e);
      term.writeln(`Error: ${e.message}`);
    }
  };

  return (
    <div id="connector">
      <Show when={!connected()}>
        <h3>Device Connection</h3>
        <button disabled={running()} onClick={connect}>
          Connect to Device
        </button>
      </Show>
    </div>
  );
};
