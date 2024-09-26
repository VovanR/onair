import Fastify from "fastify";

const STATUS_LIVETIME_MS = 5000;
const PORT = 3000;
const HOST = "0.0.0.0";
const LOGGER_ENABLED = true;

const fastify = Fastify({
  logger: LOGGER_ENABLED,
});

const Status = {
  status: false,
  timestamp: Date.now(),
  livetime: STATUS_LIVETIME_MS,
  getStatus() {
    if ((Date.now() - this.timestamp) > this.livetime) {
      return false;
    }
    return this.status;
  },
  setStatus(status) {
    this.timestamp = Date.now();
    this.status = status === true;
  },
};

/**
 * @example
 * ```sh
 * curl http://localhost:3000/api/status
 * ```
 */
fastify.get("/api/status", (request, reply) => {
  reply.send({ status: "ok", responseData: { status: Status.getStatus() } });
});

/**
 * @example
 * ```sh
 * curl -d '{"status":true}' -X POST http://localhost:3000/api/status -H "Content-Type: application/json"
 * ```
 */
fastify.post("/api/status", {
  schema: {
    body: {
      type: "object",
      required: ["status"],
      properties: {
        status: { type: "boolean" },
      },
    }
  }
}, (request, reply) => {
  Status.setStatus(request.body.status === true);
  reply.send({ status: "ok" });
});

fastify.listen({ port: PORT, host: HOST }, (err, address) => {
  if (err) {
    fastify.log.error(err);
    process.exit(1);
  }
});