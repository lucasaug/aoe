#ifndef PLAYER_H
#define PLAYER_H

#include "raylib/raylib.h"
#include "raylib/rcamera.h"


typedef struct MovementTarget {
    Vector3 destination;
    bool arrived;
} MovementTarget;

typedef struct Player {
    Camera camera;
    MovementTarget currentTarget;
} Player;

Player InitPlayer(float);

void SetPlayerTarget(Player*, const Vector3);

#endif // PLAYER_H
