#ifndef SCENE_H
#define SCENE_H

#include "player.h"
#include "raylib/raylib.h"
#include "world.h"

typedef struct Scene {
    Ground ground;
    Player player;

    int screenWidth;
    int screenHeight;

    int value;

    Vector3 otherPlayerPos;
} Scene;

void InitScene(Scene*, int, int, float);

void UpdateScene(Scene*);

void RenderScene(Scene*);

void FreeScene(const Scene*);

#endif // SCENE_H
