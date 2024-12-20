import Fastify from "fastify";
import fastifyWebsocket from "@fastify/websocket";
import fastifyStatic from "@fastify/static";
import path from "node:path";

/**
 * One second more than PC ping to prevent ESP32 led blinking
 */
const STATUS_LIFETIME_MS = 6000;
const PORT = 3000;
/**
 * Set "0.0.0.0" if started in container
 */
const HOST = "0.0.0.0";
/**
 * For debug
 */
const LOGGER_ENABLED = true;

const fastify = Fastify({
  logger: LOGGER_ENABLED,
});

fastify.register(fastifyWebsocket);
fastify.register(fastifyStatic, {
  root: path.join(import.meta.dirname, "public"),
  // prefix: "/public/", // optional: default "/"
  // constraints: { host: "example.com" }
});

class Status {
  #status = false;
  #timestamp = Date.now();
  #livetime = STATUS_LIFETIME_MS;
  constructor() { }
  getStatus() {
    if (Date.now() - this.#timestamp > this.#livetime) {
      return false;
    }
    return this.#status;
  }
  /**
   * @param {boolean} status
   */
  setStatus(status) {
    this.#timestamp = Date.now();
    this.#status = status === true;
  }
}

const cameraStatus = new Status();
const microphoneStatus = new Status();

fastify.get("/", function (req, reply) {
  reply.sendFile("index.html");
});

/**
 * @example
 * ```sh
 * curl http://localhost:3000/api/status
 * ```
 */
fastify.get("/api/status", (request, reply) => {
  reply.send({
    status: "ok",
    responseData: {
      camera: cameraStatus.getStatus(),
      microphone: microphoneStatus.getStatus(),
    },
  });
});

fastify.register(async function (fastify) {
  fastify.get("/api/ws/status", { websocket: true }, (socket) => {
    socket.on("message", (message) => {
      if (message.toString() === "getStatus") {
        socket.send(JSON.stringify({
          camera: cameraStatus.getStatus(),
          microphone: microphoneStatus.getStatus(),
        }));
      }
    });
  });
});

/**
 * @example
 * ```sh
 * curl -d '{"camera":true,"microphone":true}' -X POST http://localhost:3000/api/status -H "Content-Type: application/json"
 * ```
 */
fastify.post(
  "/api/status",
  {
    schema: {
      body: {
        type: "object",
        required: ["camera", "microphone"],
        properties: {
          camera: { type: "boolean" },
          microphone: { type: "boolean" },
        },
      },
    },
  },
  (request, reply) => {
    cameraStatus.setStatus(request.body.camera === true);
    microphoneStatus.setStatus(request.body.microphone === true);
    reply.send({ status: "ok" });
  }
);

fastify.listen({ port: PORT, host: HOST }, (err, address) => {
  if (err) {
    fastify.log.error(err);
    process.exit(1);
  }
});
