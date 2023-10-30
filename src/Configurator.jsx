import { createSignal, For, Show } from "solid-js";
import { connected, esploader, running, term } from "./index";
import { elements, configPath } from "./config";

export const Configurator = () => {
  let config = elements;
  let enc = new TextEncoder();

  const [show, setShow] = createSignal(false);

  const upload = async () => {
    if (esploader().transport.device.writable) {
      const preparedConfig = JSON.stringify(config, null, 2);
      const writer = esploader().transport.device.writable.getWriter();
        writer.write(enc.encode(`/file-remove ${configPath}\n`));
        const lines = preparedConfig.split("\n");
        for (const line of lines) {
          writer.write(enc.encode(`/file-append ${configPath} ${line}\n`));
          await new Promise((resolve) => setTimeout(resolve, 100));
        }
        writer.releaseLock();
        term.writeln("Config upload complete!");
    }
  };


  const read = async () => {
    await esploader().transport.write(enc.decode(`/file-read ${configPath}\n`));
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
        <button onClick={() => setShow(!show())}>Configure</button>
        <Show when={show()}>
            <button disabled={running()} onClick={upload}>Upload config</button>
            <button disabled={running()} onClick={read}>Read config</button>
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
          </Show>
      </Show>
    </div>
  );
};
