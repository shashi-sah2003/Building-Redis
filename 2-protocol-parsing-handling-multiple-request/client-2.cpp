#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>


const size_t k_max_size = 1024;

//function to print a message to stderr
static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

//Function to print a message to stderr
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

//write_full() function writes to the kernel until it writes exactly n bytes.
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

static int32_t query(int fd, const char *text){
    uint32_t len = (uint32_t)strlen(text);

    if(len > k_max_size){
        return -1;
    }

    char wbuff[ 4 + k_max_size];
    memcpy(wbuff, &len, 4);
    memcpy(&wbuff[4], text, len);

    if(int32_t err = write_all(fd, wbuff, 4+len)){
        return err;
    }

    //4 bytes header
    char rbuff[4+k_max_size+1];
    errno=0;
    int32_t err = read_full(fd, rbuff, 4);
    if(err){
        if(errno = 0){
            msg("EOF");
        }
        else{
            msg("read() error");
        }
        return err;
    }

    memcpy(&len, rbuff, 4);
    if(len > k_max_size){
        msg("too long");
        return -1;
    }

    //reply body
    err = read_full(fd, &rbuff[4], len);
    if(err){
        msg("read() error");
        return err;
    }

    //do something
    rbuff[4+len] = '\0';
    printf("server says: %s\n", &rbuff[4]);
    return 0;
}


int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    // Set up the server address structure
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(6378);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1

    // Connect to the server
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("connect");
    }

    // multiple requests
    int32_t err = query(fd, "hello1");
    if (err) {
        goto L_DONE;
    }
    err = query(fd, "hello2");
    if (err) {
        goto L_DONE;
    }
    err = query(fd, "hello3");
    if (err) {
        goto L_DONE;
    }

L_DONE:
    close(fd);
    return 0;
}