#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT "1235"
#define BACKLOG 10

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;


    printf("1");
    if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    printf("2");
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        fprintf(stderr, "error creating socket: %d\n", errno);
        return 3;
    }

    printf("3");
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        fprintf(stderr, "error binding: %d\n", errno);
        return 4;
    }

    printf("almos");
    if (listen(sockfd, BACKLOG) == -1) {
        fprintf(stderr, "error listening: %d\n", errno);
        return 5;
    }

    printf("Listening...");
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    int newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    if (newfd == -1) {
        fprintf(stderr, "error accepting: %d\n", errno);
        return 6;
    }
    close(sockfd);

    printf("Connected!");
    int counter = 0;
    while (1) {
        if (send(newfd, &counter, sizeof(int), 0) == -1) {
            fprintf(stderr, "error sending: %d\n", errno);
            return 7;
        }
        sleep(5);
        counter++;
    }

    close(newfd);
    freeaddrinfo(res);

    return 0;
}
