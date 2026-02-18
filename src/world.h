#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>

#include "raylib/raylib.h"
#include "raylib/rcamera.h"

#define MAX_HEIGHT 1000

typedef struct Ground {
    Vector3 origin;
    Model model;
} Ground;

void InitGround(Ground*, int32_t, int32_t);

void FreeGround(const Ground*);

void DrawGround(const Ground*);

#endif // WORLD_H
