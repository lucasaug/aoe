#ifndef SCENE_H
#define SCENE_H

#include "player.h"
#include "world.h"

typedef struct Scene {
    Ground ground;
    Player player;

    int screenWidth;
    int screenHeight;

    int value;
} Scene;

void InitScene(Scene*, int, int, float);

void UpdateScene(Scene*);

void RenderScene(Scene*);

void FreeScene(const Scene*);

#endif // SCENE_H
