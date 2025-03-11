import { createSignal, For, Show } from "solid-js";
import { connected, esploader, running, term } from "./index";
import { elements, configPath } from "./config";

export const Configurator = () => {
  let config = elements;
  let enc = new TextEncoder();

  const [show, setShow] = createSignal(false);

  const upload = async () => {
    await esploader().hard_reset();
    await new Promise((resolve) => setTimeout(resolve, 1000));
    if (esploader().transport.device.writable) {
      const preparedConfig = JSON.stringify(config, null, 2);
      const writer = esploader().transport.device.writable.getWriter();
        writer.write(enc.encode(`/file-remove\n`));
        const lines = preparedConfig.split("\n");
        for (const line of lines) {
          writer.write(enc.encode(`/file-append ${line}\n`));
          await new Promise((resolve) => setTimeout(resolve, 200));
        }
        writer.write(enc.encode(`/config-done\n`));
        await new Promise((resolve) => setTimeout(resolve, 200));
        writer.releaseLock();
        term.writeln("Config upload complete!");
    }
  };


  const deleteConfig = async () => {
    await esploader().hard_reset();
    await new Promise((resolve) => setTimeout(resolve, 1000));
    const writer = esploader().transport.device.writable.getWriter();
    await writer.write(enc.encode(`/file-remove\n`));
    await new Promise((resolve) => setTimeout(resolve, 1000));
    await writer.write(enc.encode(`/config-done\n`));
    await new Promise((resolve) => setTimeout(resolve, 1000));
    writer.releaseLock();
  };


  const read = async () => {
    const reader = esploader().transport.device.readable.getReader();
    await esploader().transport.write(enc.encode(`/file-read\n`));
    console.log("reading config");
    console.log("config", reader.read());
  };

  const updateFormValue = (e) => {
    const value = e.currentTarget.value;
    const name = e.currentTarget.name;
    config.forEach((element) => {
      if (element.name === name) {
        element.value = value;
      }
    });
  };

  return (
    <div id="configurator">
      <Show when={connected()}>
        <h3>Configure Device</h3>
        <button onClick={() => setShow(!show())}>Show Configuration Options</button>
        <Show when={show()}>
            <For each={config}>
              {(element) => (
                <div class="element">
                  <label for={element.name}>{element.label}</label> <br />
                  <input
                    onChange={updateFormValue}
                    value={element.value}
                    id={element.name}
                    name={element.name}
                    type={element.type}
                  />
                </div>
              )}
            </For>
            <button disabled={running()} onClick={upload}>Upload config</button>
            <button disabled={running()} onClick={deleteConfig}>Delete config</button>
          </Show>
      </Show>
    </div>
  );
};
