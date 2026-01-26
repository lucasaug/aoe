#include <stdint.h>
#include <stdlib.h>

#include "raylib.h"
#include "rcamera.h"

#define MAX_HEIGHT 20
#define CELL_LENGTH 10

typedef struct Ground {
    Vector3 origin;
    int32_t grid_width, grid_height;
    int32_t** heights;
} Ground;

void GenerateGround(int32_t width, int32_t height, Ground* ground) {
    ground->origin = (Vector3){.x = 0.f, .y = 0.f, .z = 0.f};

    ground->heights = (int32_t**) malloc(ground->grid_width * sizeof(int32_t*));
    for(int32_t i = 0; i < ground->grid_width; i++) {
        ground->heights[i] = (int32_t*) malloc(ground->grid_height * sizeof(int32_t));

        for(int32_t j = 0; j < ground->grid_height; j++) {
            ground->heights[i][j] = GetRandomValue(0, MAX_HEIGHT);
        }
    }
}

void FreeGround(Ground* ground) {
    for(int32_t i = 0; i < ground->grid_width; i++) {
        free(ground->heights[i]);
    }
    free(ground->heights);
}

void DrawGround(const Ground* ground) {
    for(int32_t i = 0; i < ground->grid_width; i++) {
        int32_t point_count = 2;
        Vector2* points = (Vector2*) malloc(point_count * sizeof(Vector2));

        // DrawTriangleStrip3D(points, 7, RED);
    }

}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "My weird game");

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;

    Ground ground;
    GenerateGround(10, 10, &ground);

    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advanced camera controls, it's recommended to compute camera movement manually
        UpdateCamera(&camera, cameraMode);                  // Update camera
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                // DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
                // DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
                // DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
                // DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

                // Draw some cubes around
                /* for (int i = 0; i < MAX_COLUMNS; i++)
                {
                    DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
                    DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
                } */
                DrawGround(&ground);

            EndMode3D();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
