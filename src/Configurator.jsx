import { For, Show } from "solid-js";
import { config, setConfig, connected, esploader, running } from "./index";

export const Configurator = () => {
  let enc = new TextEncoder();

  const upload = async () => {
    await reset();
    if (esploader().transport.device.writable) {
      // Filter out heading elements before uploading
      const uploadConfig = config().filter((e) => e.type !== "heading");
      const preparedConfig = JSON.stringify(uploadConfig, null, 2);
      const writer = esploader().transport.device.writable.getWriter();
      writer.write(enc.encode(`/file-remove\n`));
      const lines = preparedConfig.split("\n");
      for (const line of lines) {
        writer.write(enc.encode(`/file-append ${line}\n`));
        await new Promise((resolve) => setTimeout(resolve, 100));
      }
      await writer.write(enc.encode(`/config-done\n`));
      await new Promise((resolve) => setTimeout(resolve, 300));
      writer.releaseLock();
    }
  };

  const reset = async () => {
    await esploader().hard_reset();
    await new Promise((resolve) => setTimeout(resolve, 1000));
  };

  const deleteConfig = async () => {
    await reset();
    const writer = esploader().transport.device.writable.getWriter();
    await writer.write(enc.encode(`/file-remove\n`));
    await new Promise((resolve) => setTimeout(resolve, 1000));
    await writer.write(enc.encode(`/config-done\n`));
    await new Promise((resolve) => setTimeout(resolve, 1000));
    writer.releaseLock();
  };

  const read = async () => {
    await reset();
    const writer = esploader().transport.device.writable.getWriter();
    await writer.write(enc.encode(`/file-read\n`));
    await new Promise((resolve) => setTimeout(resolve, 5000));
    await writer.write(enc.encode(`/config-done\n`));
    await new Promise((resolve) => setTimeout(resolve, 1000));
    writer.releaseLock();
  };

  const updateFormValue = (e) => {
    const value = e.currentTarget.value;
    const name = e.currentTarget.name;
    const cfg = config()
    cfg.forEach((element) => {
      if (element.name === name && element.type !== "heading") {
        element.value = value;
      }
    });
    setConfig(null)
    setConfig(cfg)
  };

  return (
    <div id="configurator">
      <Show when={connected()}>
        <h3>Configure Device</h3>
        <button disabled={running()} onClick={upload}>Upload config</button>
        <button disabled={running()} onClick={read}>Read config</button>
        <button disabled={running()} onClick={deleteConfig}>Delete config</button>
        <For each={config()}>
          {(element) => (
            <>
                <Show when={element.type === "heading"}>
                  <div style="padding-bottom: 10px;" class="element">
                    <br />
                    <strong>{element.label}</strong>
                  </div>
                </Show>
                <Show when={element.type !== "heading"}>
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
                </Show>
            </>
          )}
        </For>
      </Show>
    </div>
  );
};
