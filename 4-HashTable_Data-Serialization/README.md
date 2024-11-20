# HashTable Data Serialization

This project demonstrates storing key-value pairs in a hash map data structure with data serialization.

## Features

1. **Double Hashtable for Resizing**: 
    - Uses two hashtables (`ht1` & `ht2`).
    - When the load factor exceeds the maximum, resizing begins, and nodes are moved from `ht2` to `ht1`.

2. **Server Commands**:
    - The server listens for commands like `get`, `set`, `del`, and `keys`, each corresponding to operations on the underlying key-value store.

3. **Data Serialization**:
    - Generalized encoding to handle different types of data.
    - Supports getting a list of strings as a response.

## Files

- `server-4.cpp`: Contains the server-side implementation, including handling connections, processing requests, and managing the hash table.
- `client-4.cpp`: Contains the client-side implementation for sending requests to the server and handling responses.
- `hashtable.cpp`: Contains the implementation of the hash table, including insertion, lookup, and resizing logic.
- `hashtable.h`: Header file for the hash table implementation.

## How to Run

1. **Compile the Server**:
    ```sh
    g++ -o server server-4.cpp hashtable.cpp
    ```

2. **Compile the Client**:
    ```sh
    g++ -o client client-4.cpp
    ```

3. **Run the Server**:
    ```sh
    ./server
    ```

4. **Run the Client**:
    ```sh
    ./client <command> [arguments]
    ```

    Example commands:
    - `./client set key value`
    - `./client get key`
    - `./client del key`
    - `./client keys`
