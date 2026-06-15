#include "messages.h"
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT "1235"
#define BACKLOG 10

unsigned int conn_counter = 0;
PlayerPosition posList[2];

typedef struct ThreadArgs {
    int sockfd;
    unsigned int playerOffset;
} ThreadArgs;

void *handle_client(void *input) {
    ThreadArgs* args = (ThreadArgs*)input;
    unsigned int id;
    bool gotId = false;

    int sockfd = args->sockfd;
    int playerOffset = args->playerOffset;

    printf("Connected!\n");
    fflush(stdout);

    while (1) {
        printf("sockfd: %d\n", sockfd);
        fflush(stdout);

        enum MessageType type;
        int result = recv(sockfd, &type, sizeof(enum MessageType), 0);

        if (result != -1) {
            void* msg = recvMessage(sockfd, type);

            if (msg == NULL) {
                printf("Error! recvMessage result is NULL");
                fflush(stdout);
                break;
            }

            if (type != PLAYER_POSITION) {
                printf("Error! Unknown message type: %d", type);
                fflush(stdout);
                break;
            }

            PlayerPosition* playerPosMsg = (PlayerPosition*) msg;
            printf("Got player position message (id=%u):\n", playerPosMsg->id);
            printf("Position:\n");
            printf("x: %f\n", playerPosMsg->position.x);
            printf("y: %f\n", playerPosMsg->position.y);
            printf("z: %f\n", playerPosMsg->position.z);
            posList[playerOffset] = *playerPosMsg;

            fflush(stdout);
        } else {
            fprintf(stderr, "Error in thread: %d\n", errno);
            fflush(stderr);
            break;
        }

        for (int i = 0; i < conn_counter; i++) {
            int sendResult = sendMessage(sockfd, PLAYER_POSITION, (void*)(&posList[i]));
            if (sendResult == -1) {
                fprintf(stderr, "Error in sendMessage: \n");
                fflush(stderr);
                break;
            }
        }
        /* if (send(sockfd, &counter, sizeof(int), 0) == -1) {
            fprintf(stderr, "error sending: %d\n", errno);
            break;
        }
        printf("Sent the value %d\n", counter);
        fflush(stdout);
        sleep(5);
        counter++; */
    }

    close(sockfd);
}

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        fprintf(stderr, "error creating socket: %d\n", errno);
        return 3;
    }

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        fprintf(stderr, "error binding: %d\n", errno);
        return 4;
    }

    if (listen(sockfd, BACKLOG) == -1) {
        fprintf(stderr, "error listening: %d\n", errno);
        return 5;
    }

    printf("Listening...\n");
    fflush(stdout);
    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    pthread_t tid[2];

    while(1) {
        int newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
        if (newfd == -1) {
            fprintf(stderr, "error accepting: %d\n", errno);
            return 6;
        }

        // TODO keeping this in the stack might be a bad idea
        ThreadArgs args;
        args.sockfd = newfd;
        args.playerOffset = conn_counter;

        pthread_create(&tid[conn_counter], NULL, handle_client, (void*)&args);
        ++conn_counter;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    close(sockfd);

    freeaddrinfo(res);

    return 0;
}
