#include <stdint.h>

#include "raylib/raylib.h"
#include "raylib/raymath.h"

#include "hud.h"
#include "player.h"
#include "world.h"

#define TARGET_TOLERANCE 1


int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "My weird game");

    Player player = InitPlayer(MAX_HEIGHT + 10.);

    Ground ground;
    GenerateGround(10000, 10000, &ground);

    DisableCursor();
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose()) {
        Vector2 md = GetMouseDelta();
        Vector2 screenCenter = { screenWidth/2.0f, screenHeight/2.0f };

        RayCollision groundCollision = {0};
        bool pressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        if (pressed) {
            Vector2 screenCenter = { screenWidth/2.0f, screenHeight/2.0f };
            Ray clickRay = GetScreenToWorldRay(screenCenter, player.camera);
            clickRay.position.y += 50;

            groundCollision = GetRayCollisionMesh(
                clickRay, ground.model.meshes[0], ground.model.transform
            );

            if (groundCollision.hit) {
                SetPlayerTarget(&player, groundCollision.point);
            }
        }

        float forwardsOffset = 0.;
        float rightOffset = 0.;
        if (!player.currentTarget.arrived) {
            Vector3 direction = Vector3Subtract(
                player.currentTarget.destination,
                player.camera.position
            );
            float distance = Vector2Length((Vector2) { direction.x, direction.z });

            if (distance < TARGET_TOLERANCE) {
                player.currentTarget.arrived = true;

            } else {
                // Normalize to get the unit direction vector
                direction = Vector3Normalize(direction);

                forwardsOffset = Vector3DotProduct(direction, GetCameraForward(&player.camera));
                rightOffset = Vector3DotProduct(direction, GetCameraRight(&player.camera));
            }
        }

        Ray ray = {
            .position=player.camera.position,
            .direction={.x=0., .y=-1., .z=0.}
        };
        ray.position.y += 10.;

        float yOffset = 0.;
        RayCollision collision = {0};
        for (int i = 0; i < ground.model.meshCount; i++) {
            ray.direction.y = -1;
            collision = GetRayCollisionMesh(
                ray, ground.model.meshes[i], ground.model.transform
            );

            if (collision.hit) {
                yOffset = (collision.point.y + 10) - player.camera.position.y;
                break;
            }

        }

        UpdateCameraPro(&player.camera,
            (Vector3) { forwardsOffset, rightOffset, yOffset },
            (Vector3) { md.x * 0.05f, md.y * 0.05f, 0.0f },
            0.0f
        );
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(player.camera);
                DrawGround(&ground);

                if (!player.currentTarget.arrived) {
                    DrawCube(player.currentTarget.destination, 10, 10, 10, RED);
                }

            EndMode3D();

            // DrawText(TextFormat("Ground origin:"), 200, 60, 20, RED);
            // DrawText(TextFormat("Position X: %f", ground.origin.x), 200, 80, 20, RED);
            // DrawText(TextFormat("Position Y: %f", ground.origin.y), 200, 100, 20, RED);
            // DrawText(TextFormat("Position Z: %f", ground.origin.z), 200, 120, 20, RED);
            DrawHUD(ray, player.camera);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    FreeGround(&ground);
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
