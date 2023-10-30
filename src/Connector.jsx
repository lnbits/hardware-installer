import { ESPLoader, Transport } from "esptool-js";
import {
  term,
  running,
  setRunning,
  connected,
  setConnected,
  esploader,
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

  const disconnect = async () => {
    await esploader().transport.disconnect();
    await esploader().transport.waitForUnlock(1500);
    setConnected(false);
    setEsploader(null);
    term.clear();
  };

  const reconnect = async () => {
    setRunning(true);
    await esploader().transport.waitForUnlock(1500);
    await esploader().transport.disconnect();
    await esploader().transport.waitForUnlock(1500);
    term.clear();
    await esploader().connect();
    await esploader().run_stub();
    setRunning(false);
  };

  const reset = async () => {
    await esploader().hard_reset();
  };

  return (
    <div id="connector">
      <h2>Connector</h2>
      <Show when={!connected()}>
        <button disabled={running()} onClick={connect}>Connect</button>
      </Show>
      <Show when={connected()}>
        <button disabled={running()} onClick={disconnect}>Disconnect</button>
        <button disabled={running()} onClick={reconnect}>Reconnect</button>
        <button disabled={running()} onClick={reset}>Reset</button>
      </Show>
    </div>
  );
};
