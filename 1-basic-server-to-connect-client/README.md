# Basic Server to Connect Client

This folder contains the source code for a basic client-server application. The server listens for incoming client connections and responds to client messages. The client connects to the server, sends a message, and receives a response.

## Files

- `client-1.cpp`: Contains the implementation of the client-side application.
- `server-1.cpp`: Contains the implementation of the server-side application.

## How to Build

To compile the client and server programs, use the following commands:

```sh
g++ -o client client-1.cpp
g++ -o server server-1.cpp
```

## How to Run

1. Start the server:
    ```sh
    ./server
    ```

2. In a separate terminal, start the client:
    ```sh
    ./client
    ```

## Description

### Server (`server-1.cpp`)

- The server creates a socket and binds it to a specific port.
- It listens for incoming client connections.
- Upon accepting a connection, it reads a message from the client, prints it, and sends a response back to the client.
- The server handles one client connection at a time.

### Client (`client-1.cpp`)

- The client creates a socket and connects to the server.
- It sends a message to the server.
- It reads the response from the server and prints it.

