#include "hud.h"
#include "player.h"
#include "scene.h"

#include "raylib/raymath.h"


#define TARGET_TOLERANCE 1

// TODO move
Ray ray;

void InitScene(
    Scene* scene,
    int screenWidth, 
    int screenHeight,
    float playerHeight
) {
    scene->screenWidth = screenWidth;
    scene->screenHeight = screenHeight;

    InitPlayer(&(scene->player), playerHeight);
    InitGround(&scene->ground, 10000, 10000);
}

void UpdateScene(Scene* scene) {
    const int screenWidth = scene->screenWidth;
    const int screenHeight = scene->screenHeight;

    Vector2 md = GetMouseDelta();
    Vector2 screenCenter = { scene->screenWidth/2.0f, screenHeight/2.0f };

    RayCollision groundCollision = {0};
    bool pressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    if (pressed) {
        Vector2 screenCenter = { screenWidth/2.0f, screenHeight/2.0f };
        Ray clickRay = GetScreenToWorldRay(screenCenter, scene->player.camera);
        clickRay.position.y += 50;

        groundCollision = GetRayCollisionMesh(
            clickRay, scene->ground.model.meshes[0], scene->ground.model.transform
        );

        if (groundCollision.hit) {
            SetPlayerTarget(&scene->player, groundCollision.point);
        }
    }

    float forwardsOffset = 0.;
    float rightOffset = 0.;
    if (!scene->player.currentTarget.arrived) {
        Vector3 direction = Vector3Subtract(
            scene->player.currentTarget.destination,
            scene->player.camera.position
        );
        float distance = Vector2Length((Vector2) { direction.x, direction.z });

        if (distance < TARGET_TOLERANCE) {
            scene->player.currentTarget.arrived = true;

        } else {
            // Normalize to get the unit direction vector
            direction = Vector3Normalize(direction);

            forwardsOffset = Vector3DotProduct(direction, GetCameraForward(&scene->player.camera));
            rightOffset = Vector3DotProduct(direction, GetCameraRight(&scene->player.camera));
        }
    }

    ray = (Ray){
        .position=scene->player.camera.position,
        .direction={.x=0., .y=-1., .z=0.}
    };
    ray.position.y += 10.;

    float yOffset = 0.;
    RayCollision collision = {0};
    for (int i = 0; i < scene->ground.model.meshCount; i++) {
        ray.direction.y = -1;
        collision = GetRayCollisionMesh(
            ray, scene->ground.model.meshes[i], scene->ground.model.transform
        );

        if (collision.hit) {
            yOffset = (collision.point.y + 10) - scene->player.camera.position.y;
            break;
        }

    }

    UpdateCameraPro(&scene->player.camera,
        (Vector3) { forwardsOffset, rightOffset, yOffset },
        (Vector3) { md.x * 0.05f, md.y * 0.05f, 0.0f },
        0.0f
    );
}

void RenderScene(Scene* scene) {
    BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(scene->player.camera);
            DrawGround(&scene->ground);

            if (!scene->player.currentTarget.arrived) {
                DrawCube(scene->player.currentTarget.destination, 10, 10, 10, RED);
            }

        EndMode3D();

        // DrawText(TextFormat("Ground origin:"), 200, 60, 20, RED);
        // DrawText(TextFormat("Position X: %f", ground.origin.x), 200, 80, 20, RED);
        // DrawText(TextFormat("Position Y: %f", ground.origin.y), 200, 100, 20, RED);
        // DrawText(TextFormat("Position Z: %f", ground.origin.z), 200, 120, 20, RED);
        DrawHUD(ray, scene->player.camera);

    EndDrawing();
}
