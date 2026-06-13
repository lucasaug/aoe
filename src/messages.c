#include "messages.h"
#include "raylib/raylib.h"
#include <stdlib.h>
#include <sys/socket.h>

void* recvMessage(int sockfd, enum MessageType type) {
    if (type == PLAYER_POSITION) {
        float x, y, z;
        unsigned int id;
        recv(sockfd, &x, sizeof(float), 0);
        recv(sockfd, &y, sizeof(float), 0);
        recv(sockfd, &z, sizeof(float), 0);
        recv(sockfd, &id, sizeof(unsigned int), 0);

        PlayerPosition* msg = malloc(sizeof(PlayerPosition));

        // TODO do actual deserialization here
        msg->position.x = x;
        msg->position.y = y;
        msg->position.z = z;
        msg->id = id;

        return msg;
    }

    return NULL;
}

void freeMessage(void* msg) {
    free(msg);
}


int sendMessage(int sockfd, enum MessageType type, void* msg) {
    if (type == PLAYER_POSITION) {
        PlayerPosition* playerPosMsg = (PlayerPosition*) msg;

        // TODO do actual serialization here
        send(sockfd, &type, sizeof(enum MessageType), 0);
        send(sockfd, &(playerPosMsg->position.x), sizeof(float), 0);
        send(sockfd, &(playerPosMsg->position.y), sizeof(float), 0);
        send(sockfd, &(playerPosMsg->position.z), sizeof(float), 0);
        send(sockfd, &(playerPosMsg->id), sizeof(unsigned int), 0);

        return sizeof(PlayerPosition);
    }

    return -1;
}
