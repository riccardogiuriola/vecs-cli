# vecs-cli

**vecs-cli** is a lightweight, synchronous Command Line Interface (CLI) client written in C for the **VECS** (Vector Semantic Cache) server.

It acts as a REPL (Read-Eval-Print Loop) tool, allowing developers to interact with the VECS server using human-readable commands, handling the binary RESP (Redis Serialization Protocol) encoding and decoding transparently.

## 🚀 Features

- **REPL Interface**: Interactive shell to send commands to the server.
- **Smart Parsing**: Handles quoted strings automatically (e.g., `"Hello World"` is treated as a single argument), so you don't need to manually calculate byte lengths or hex codes.
- **RESP Protocol**: Native implementation of the serialization protocol used by VECS.
- **Lightweight**: Zero external dependencies (uses standard C libraries).

## 📂 Project Structure

```text
vecs-cli/
├── include/
│   └── cli.h        # Shared header definitions
├── src/
│   ├── main.c       # Main entry point and loop
│   ├── network.c    # TCP socket connection handling
│   ├── parser.c     # Command tokenizer (handles quotes)
│   └── resp.c       # RESP protocol Encoder/Decoder
├── obj/             # Compiled object files (created during build)
└── Makefile         # Build configuration
```
