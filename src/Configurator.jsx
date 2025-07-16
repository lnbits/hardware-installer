import { createSignal, For, Show } from "solid-js";
import { connected, esploader, running, term } from "./index";
import { elements, configPath } from "./config";

export const Configurator = () => {
  let config = elements;
  let enc = new TextEncoder();

  const [show, setShow] = createSignal(false);

  const upload = async () => {
    if (esploader().transport.device.writable) {
      // Filter out heading elements before uploading
      const uploadConfig = config.filter((e) => e.type !== "heading");
      const preparedConfig = JSON.stringify(uploadConfig, null, 2);
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
      if (element.name === name && element.type !== "heading") {
        element.value = value;
      }
    });
  };

  const reset = async () => {
    await esploader().hard_reset();
  };

  return (
    <div id="configurator">
      <Show when={connected()}>
        <h3>Configure Device</h3>
        <button onClick={() => setShow(!show())}>Show Configuration Options</button>
        <Show when={show()}>
          <div>
            <button disabled={running()} onClick={reset}>
              Reset Device (Start Configuration Mode)
            </button>
          </div>
          <h2>Settings</h2>
          <For each={config}>
            {(element) => {
              if (element.type !== "heading") {
                return (
                  <div class="element">
                    <label for={element.name}>{element.label}</label> <br />
                    <input
                      onChange={updateFormValue}
                      value={element.value || ""}
                      id={element.name}
                      name={element.name}
                      type={element.type}
                    />
                  </div>
                );
              }
              else{
                return (
                  <div class="element">
                    <h4>{element.label}</h4>
                  </div>
                );
            }
            }}
          </For>
          <button disabled={running()} onClick={upload}>
            Upload config
          </button>
        </Show>
      </Show>
    </div>
  );
};
