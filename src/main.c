#include <stdint.h>

#include "raylib/raylib.h"

#include "scene.h"
#include "world.h"


int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "My weird game");

    Scene scene = {0};
    InitScene(&scene, screenWidth, screenHeight, MAX_HEIGHT + 10.);

    DisableCursor();
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose()) {
        UpdateScene(&scene);
        RenderScene(&scene);
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    FreeGround(&scene.ground);
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
