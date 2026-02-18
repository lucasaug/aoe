#include <stdint.h>

#include "raylib/raylib.h"
#include "raylib/rcamera.h"

#include "world.h"

void InitGround(Ground* ground, int32_t width, int32_t height) {
    ground->origin = (Vector3){.x = 0., .y = -100., .z = 0.};
    Image noise = GenImagePerlinNoise(500., 500., 0., 0., 4.);
    Mesh heightmapMesh = GenMeshHeightmap(noise, (Vector3){
        .x = width, .y = MAX_HEIGHT, .z = height
    });
    ground->model = LoadModelFromMesh(heightmapMesh);

    ImageColorTint(&noise, GREEN);
    Texture2D groundTexture = LoadTextureFromImage(noise);
    ground->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = groundTexture;
    UnloadImage(noise);
}

void FreeGround(const Ground* ground) {
    UnloadModel(ground->model);
}

void DrawGround(const Ground* ground) {
    DrawModel(ground->model, ground->origin, 1.0, GREEN);
}
