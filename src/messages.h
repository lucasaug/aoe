#ifndef MESSAGES_H
#define MESSAGES_H

#include "raylib/raylib.h"

enum MessageType {
    PLAYER_POSITION
};

typedef struct PlayerPosition {
    Vector3 position;
    unsigned int id;
} PlayerPosition;


void* recvMessage(int, enum MessageType);
int sendMessage(int, enum MessageType, void*);

#endif // MESSAGES_H
