#ifndef MESSAGES_H
#define MESSAGES_H

#include "raylib/raylib.h"

enum message_type {
    PLAYER_POSITION
};

typedef struct PlayerPosition {
    Vector3 position;
    unsigned int id;
} PlayerPosition;

#endif // MESSAGES_H
