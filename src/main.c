#include <stdint.h>

#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "raylib/rcamera.h"

#define MAX_HEIGHT 2000
#define TARGET_TOLERANCE 10

typedef struct Ground {
    Vector3 origin;
    Model model;
} Ground;

typedef struct MovementTarget {
    Vector3 destination;
    bool arrived;
} MovementTarget;

void GenerateGround(int32_t width, int32_t height, Ground* ground) {
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

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;
    MovementTarget currentTarget = {0};
    currentTarget.arrived = true;

    InitWindow(screenWidth, screenHeight, "My weird game");

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, MAX_HEIGHT, 0.0f };   // Camera position
    camera.target = (Vector3){ 0.0f, 0., -4.0f };    // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Ground ground;
    GenerateGround(10000, 10000, &ground);
    DisableCursor();                    // Limit cursor to relative movement inside the window

    // ShowCursor();
    // SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {

        Ray ray = {
            .position=camera.position,
            .direction={.x=0., .y=-1., .z=0.}
        };
        ray.position.y += 10.;

        RayCollision collision = {0};
        for (int i = 0; i < ground.model.meshCount; i++) {
            ray.direction.y = -1;
            collision = GetRayCollisionMesh(
                ray, ground.model.meshes[i], ground.model.transform
            );

            if (collision.hit) {
                camera.position.y = collision.point.y + 100;
                break;
            }

            // ray.direction.y = 1;
            // collision = GetRayCollisionMesh(
            //     ray, ground.model.meshes[i], ground.model.transform
            // );
            //
            // if (collision.hit) {
            //     // camera.position.y = collision.point.y;
            //     break;
            // }
        }

        bool pressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        if (pressed) {
            Ray mouseRay = GetScreenToWorldRay(GetMousePosition(), camera);

            collision = GetRayCollisionMesh(
                mouseRay, ground.model.meshes[0], ground.model.transform
            );

            if (collision.hit) {
                currentTarget.destination = collision.point;
                currentTarget.arrived = false;
            }
        }

        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advanced camera controls, it's recommended to compute camera movement manually
        //
        // CAMERA_CUSTOM = 0,              // Camera custom, controlled by user (UpdateCamera() does nothing)
        // CAMERA_FREE,                    // Camera free mode
        // CAMERA_ORBITAL,                 // Camera orbital, around target, zoom supported
        // CAMERA_FIRST_PERSON,            // Camera first person
        // CAMERA_THIRD_PERSON             // Camera third person
        // UpdateCamera(&camera, CAMERA_FIRST_PERSON);           // Update camera
        // UpdateCameraPro(&camera, (Vector3){0.,0.,-1.}, (Vector3){0., 0., 0.}, 1.);
        Vector2 md = GetMouseDelta();

        bool forward = IsKeyDown(KEY_W);
        bool backwards = IsKeyDown(KEY_S);
        bool left = IsKeyDown(KEY_A);
        bool right = IsKeyDown(KEY_D);

        // forward = backwards = left = right = false;
        Vector3 delta = (Vector3) { 0., 0., 0.};
        if (forward) {
            delta.x = 1;
        }
        if (backwards) {
            delta.x = -1;
        }
        if (left) {
            delta.y = -1;
        }
        if (right) {
            delta.y = 1;
        }

        if (!currentTarget.arrived) {

            delta.x = currentTarget.destination.x - camera.position.x;
            delta.y = currentTarget.destination.z - camera.position.z;

            if (delta.x < TARGET_TOLERANCE && delta.y < TARGET_TOLERANCE) {
                delta = (Vector3){0};
                currentTarget.arrived = true;
            } else {
                delta = Vector3Normalize(delta);
                delta.x *= 10;
                delta.y *= 10;
            }
        }

        UpdateCameraPro(&camera,
                /* { forward-backward, left-right, up-down } */ delta,
                /* { yaw, pitch, roll } */ (Vector3) { md.x * 0.05f, md.y * 0.05f, 0.0f },
                /* zoom */ 0.0f);
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

                // Vector3 endPos = camera.position;
                // endPos.y += 10.;
                // DrawCapsule(camera.position, endPos, 10., 10, 10, BLUE);
                DrawCube(camera.position, 10., 10., 10., BLUE);

            EndMode3D();

            DrawFPS(10, 10);

            DrawText(TextFormat("Ray:"), 200, 60, 20, RED);
            DrawText(TextFormat("Position X: %f", ray.position.x), 200, 80, 20, RED);
            DrawText(TextFormat("Position Y: %f", ray.position.y), 200, 100, 20, RED);
            DrawText(TextFormat("Position Z: %f", ray.position.z), 200, 120, 20, RED);

            DrawText(TextFormat("Direction X: %f", ray.direction.x), 500, 80, 20, RED);
            DrawText(TextFormat("Direction Y: %f", ray.direction.y), 500, 100, 20, RED);
            DrawText(TextFormat("Direction Z: %f", ray.direction.z), 500, 120, 20, RED);

            DrawText(TextFormat("Moving to:"), 200, 180, 20, RED);
            DrawText(TextFormat("Arrived: %d", currentTarget.arrived), 200, 200, 20, RED);
            DrawText(TextFormat("%f", currentTarget.destination.x), 200, 220, 20, RED);
            DrawText(TextFormat("%f", currentTarget.destination.y), 200, 240, 20, RED);
            DrawText(TextFormat("%f", currentTarget.destination.z), 200, 260, 20, RED);

            DrawText(TextFormat("Camera:"), 500, 180, 20, RED);
            DrawText(TextFormat("%f", camera.position.x), 500, 220, 20, RED);
            DrawText(TextFormat("%f", camera.position.y), 500, 240, 20, RED);
            DrawText(TextFormat("%f", camera.position.z), 500, 260, 20, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    FreeGround(&ground);
    //--------------------------------------------------------------------------------------

    return 0;
}
