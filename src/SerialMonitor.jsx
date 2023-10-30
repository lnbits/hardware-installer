import { createSignal, Show } from "solid-js";
import { esploader, running, setRunning, term } from "./index";

export const SerialMonitor = () => {

  const [serialRunning, setSerialRunning] = createSignal(false);


  const start = async () => {
      // await esploader().change_baud(115200);
      setRunning(true);
      setSerialRunning(true);
      while (true) {
        if (serialRunning() === false) {
          const reader = await esploader().transport.device.readable.getReader();
          reader.releaseLock();
          break;
        }
        try {
          const val = await esploader().transport.rawRead(1000);
          term.write(val);
        } catch (e) {
            if (e.message !== "Timeout") {
                console.error(e);
                term.writeln(e.message);
            }
            await new Promise((resolve) => setTimeout(resolve, 1000));
        }
      }
      console.log("exiting serial monitor");
  };

  const stop = async () => {
    setRunning(false);
    setSerialRunning(false);
    await esploader().transport.waitForUnlock();
    console.log("unlocked transport");
    term.clear();
  };

  return (
    <div id="serialmonitor">
      <h2>Serial Monitor</h2>
      <Show when={!serialRunning()}>
        <button disabled={running()} onClick={start}>Start Serial Monitor</button>
      </Show>
      <Show when={serialRunning()}>
        <button onClick={stop}>Stop Serial Monitor</button>
      </Show>
    </div>
  );
};
