import { defineConfig } from "vite";
import solidPlugin from "vite-plugin-solid";

export default defineConfig({
  plugins: [solidPlugin()],
  server: {
    cors: { origin: "*" },
    hmr: {
        overlay: false
    }
  },
  build: {
    commonjsOptions: {
      transformMixedEsModules: true,
    },
  },
});
