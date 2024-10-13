#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

//function to print a message to stderr
static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

//function to print an error message along with errno and abort the program
static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

//Hanlding communication with the client once connection is established
static void do_something(int connfd) {
    //stores the message received from the client
    char rbuf[64] = {}; 
    //read data from client
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        msg("read() error");
        return;
    }
    //print the message received from the client
    printf("client says: %s\n", rbuf);

    //response message to client
    char wbuf[] = "world";
    //writing response back to client
    write(connfd, wbuf, strlen(wbuf));
}

int main() {
    // Create a socket (IPv4, stream-based, default protocol
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    // this is needed for most server applications
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // bind the socket to an address and port
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(6378);
    addr.sin_addr.s_addr = ntohl(0);    // wildcard address 0.0.0.0

    //binding the socket to address
    int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("bind()");
    }

    // listen for upcoming connections
    rv = listen(fd, SOMAXCONN);
    if (rv) {
        die("listen()");
    }

    //infinite loop to accept and handle incoming client connections
    while (true) {
        // accept
        struct sockaddr_in client_addr = {};
        socklen_t socklen = sizeof(client_addr);

        //Accepting incoming client connection
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
        if (connfd < 0) {
            continue;   // error
        }

        do_something(connfd);
        close(connfd);
    }

    return 0;
}