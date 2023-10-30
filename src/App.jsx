import { Show, onMount } from "solid-js";
import { connected, esploader, running, term } from "./index";
import { Connector } from "./Connector";
import { Configurator } from "./Configurator";
import { Programmer } from "./Programmer";
// import { SerialMonitor } from "./SerialMonitor";


export const App = () => {

  // serial monitor
  onMount(async () => {
      while (true) {
        if (!connected() || running()) {
            // if (esploader()) {
            //     await esploader().transport.device.readable.getReader().releaseLock();
            // }
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
      <div className="flex">
          <Connector />
          <Show when={connected()}>
            <Programmer />
            <Configurator />
          </Show>
      </div>
      <button style="margin-top: 12px;" onClick={() => term.clear()}>Clear Terminal</button>
    </div>
  );
};
