#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <assert.h>

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

//The read_full() function reads from the kernel until it gets exactly n bytes.
static int32_t read_full(int fd, char* buf, size_t n){
    while(n>0){
        //The read() syscall just returns whatever data is available in the kernel
        ssize_t rv = read(fd, buf, n);
        if(rv<=0){
            return -1;
        }
        
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }

    return 0;
}

//write_all() function writes to the kernel until it writes exactly n bytes.
static int32_t write_all(int fd, const char* buf, size_t n){
    while(n>0){
        ssize_t rv = write(fd, buf, n);
        if(rv<=0){
            return -1;
        }

        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

const size_t k_max_size = 1024;

//The one_request function parses only one request and responds, 
//until something bad happens or the client connection is lost
static int32_t one_request(int connfd){

    //4 bytes header
    char rbuff[4+k_max_size+1];
    errno=0;
    int32_t err = read_full(connfd, rbuff, 4);
    if(err){
        if(errno=0){
            msg("EOF");
        }
        else{
            msg("read() error");
        }
        return err;
    }

    uint32_t len=0;

    //memcpy is used to copy the 4 bytes of memory from rbuff to len
    memcpy(&len, rbuff, 4);
    if(len > k_max_size){
        msg("too long");
        return -1;
    }

    //request body
    err = read_full(connfd, &rbuff[4], len);
    if(err){
        msg("read() error");
        return err;
    }

    //do something
    rbuff[4+len] = '\0';
    printf("client says: %s\n", &rbuff[4]);

    //reply using the same protocol
    const char reply[] = "world";
    char wbuff[4+sizeof(reply)];
    len = (uint32_t)strlen(reply);
    memcpy(wbuff, &len, 4);
    memcpy(&wbuff[4], reply, len);
    return write_all(connfd, wbuff, 4+len);
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

        // only serves one client connection at once
        while (true) {
            //the server only serves one client connection at once
            int32_t err = one_request(connfd);
            if (err) {
                break;
            }
        }
        close(connfd);
    }

    return 0;
}