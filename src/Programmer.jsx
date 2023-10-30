import { esploader, term, connected, running, setRunning } from "./index";

import data from "./versions.json";
import { addressesAndFiles } from "./config.js";


export const Programmer = () => {

  const erase = async () => {
    try {
      setRunning(true);
      await new Promise((resolve) => setTimeout(resolve, 3000));
      await esploader().erase_flash();
    } catch (e) {
      console.error(e);
      term.writeln(`Error: ${e.message}`);
    } finally {
      setRunning(false);
    }
  };

  const flash = async (version, device) => {
    console.log("flashing", version, device);
    // await esploader().change_baud(921600);
    const fileArray = [];
    for (const item of addressesAndFiles) {
      console.log(`Address: ${item.address}, File Name: ${item.fileName}`);
      const response = await fetch(`/src/firmware/${device}/${version}/${item.fileName}`);
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const fileBlob = await response.blob();
      const fileData = await new Promise((resolve, reject) => {
        const reader = new FileReader();
        reader.onloadend = () => resolve(reader.result);
        reader.onerror = reject;
        reader.readAsBinaryString(fileBlob);
      });
      fileArray.push({ data: fileData, address: item.address });
    }
    try {
      setRunning(true);
      await new Promise((resolve) => setTimeout(resolve, 3000));
      await esploader().write_flash({
        fileArray: fileArray,
          flashSize: "keep",
          eraseAll: false,
          compress: true,
      });
      await new Promise((resolve) => setTimeout(resolve, 100));
      await esploader().transport.setDTR(false);
      await new Promise((resolve) => setTimeout(resolve, 100));
      await esploader().transport.setDTR(true)
    } catch (e) {
      console.error(e);
    } finally {
      setRunning(false);
    }
  };

  return (
    <div id="programmer">
      <h2>Programmer</h2>
      <Show when={connected()}>
        <button disabled={running()} onClick={erase}>Erase</button>
        <hr />
        <For each={data.devices} fallback={<div>Loading...</div>}>
          {(device) => (
            <div className="device">
              <h3>{device}</h3>
              <For each={data.versions} fallback={<div>Loading...</div>}>
                {(version) => (
                  <div class="version">
                    <label>
                      Bitcoinswitch ({device}) ({version})
                    </label>
                    <button disabled={running()} onClick={() => flash(version, device)}>Flash</button>
                  </div>
                )}
              </For>
            </div>
          )}
        </For>
      </Show>
    </div>
  );
};
