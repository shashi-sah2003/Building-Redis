# Event Loop and Nonblocking IO

Now that we have handled multiple requests from a client to a server, we need to handle multiple clients to a server.

## 3 Ways to Deal with Concurrent Connections

1. **Forking**: Creates new processes for each client connection to achieve concurrency.
2. **Multi-threading**: Uses threads instead of processes.
3. **Event loops**: Uses polling and nonblocking IO and usually runs on a single thread.

Due to the overhead of processes and threads, most modern production-grade software uses event loops for networking.

In an event loop, we poll the operation to tell us which file descriptor (fd) can be operated on without blocking.

Generally, in blocking mode:
- `read` blocks the caller when no data is in the kernel,
- `write` blocks when the write buffer is full,
- `accept` blocks when there are no new connections in the kernel queue.

In nonblocking mode, these operations either succeed without blocking or fail with the errno `EAGAIN`, which means "not ready".
