#include "messages.h"
#include "raylib/raylib.h"
#include <stdlib.h>
#include <sys/socket.h>

int recvMessage(int sockfd, enum MessageType type, void* msg) {
    if (type == PLAYER_POSITION) {
        float x, y, z;
        unsigned int id;
        recv(sockfd, &x, sizeof(float), 0);
        recv(sockfd, &y, sizeof(float), 0);
        recv(sockfd, &z, sizeof(float), 0);
        recv(sockfd, &id, sizeof(unsigned int), 0);

        // TODO do actual deserialization here
        Vector3 position = (Vector3) { x, y, z };
        PlayerPosition* playerPos = (PlayerPosition*)
            malloc(sizeof(PlayerPosition));

        playerPos->position.x = x;
        playerPos->position.y = y;
        playerPos->position.z = z;
        playerPos->id = id;

        msg = (void*)playerPos;

        return sizeof(PlayerPosition);
    }

    return -1;
}


int sendMessage(int sockfd, enum MessageType type, void* msg) {
    if (type == PLAYER_POSITION) {
        float x, y, z;
        unsigned int id;
        recv(sockfd, &x, sizeof(float), 0);
        recv(sockfd, &y, sizeof(float), 0);
        recv(sockfd, &z, sizeof(float), 0);
        recv(sockfd, &id, sizeof(unsigned int), 0);

        // TODO do actual deserialization here
        Vector3 position = (Vector3) { x, y, z };
        PlayerPosition* playerPos = (PlayerPosition*)
            malloc(sizeof(PlayerPosition));

        playerPos->position.x = x;
        playerPos->position.y = y;
        playerPos->position.z = z;
        playerPos->id = id;

        msg = (void*)playerPos;

        return sizeof(PlayerPosition);
    }

    return -1;
}
