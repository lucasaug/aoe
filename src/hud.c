#include "raylib/raylib.h"
#include "raylib/rcamera.h"

#include "hud.h"

void DrawHUD(Ray ray, Camera camera) {
    Vector2 middle = (Vector2){GetScreenWidth() / 2., GetScreenHeight() / 2.};

    int crosshairOffset = CROSSHAIR_LENGTH / 2.;
    DrawRectangle(
        middle.x - crosshairOffset,
        middle.y - 1.,
        CROSSHAIR_LENGTH,
        2,
        GRAY
    );

    DrawRectangle(
        middle.x - 1.,
        middle.y - crosshairOffset,
        2,
        CROSSHAIR_LENGTH,
        GRAY
    );

    DrawFPS(10, 10);

    DrawText(TextFormat("Ray:"), 200, 60, 20, RED);
    DrawText(TextFormat("Position X: %f", ray.position.x), 200, 80, 20, RED);
    DrawText(TextFormat("Position Y: %f", ray.position.y), 200, 100, 20, RED);
    DrawText(TextFormat("Position Z: %f", ray.position.z), 200, 120, 20, RED);

    DrawText(TextFormat("Direction X: %f", ray.direction.x), 500, 80, 20, RED);
    DrawText(TextFormat("Direction Y: %f", ray.direction.y), 500, 100, 20, RED);
    DrawText(TextFormat("Direction Z: %f", ray.direction.z), 500, 120, 20, RED);

    // DrawText(TextFormat("Moving to:"), 200, 180, 20, RED);
    // DrawText(TextFormat("Arrived: %d", currentTarget.arrived), 200, 200, 20, RED);
    // DrawText(TextFormat("%f", currentTarget.destination.x), 200, 220, 20, RED);
    // DrawText(TextFormat("%f", currentTarget.destination.y), 200, 240, 20, RED);
    // DrawText(TextFormat("%f", currentTarget.destination.z), 200, 260, 20, RED);

    DrawText(TextFormat("Camera:"), 500, 180, 20, RED);
    DrawText(TextFormat("%f", camera.position.x), 500, 220, 20, RED);
    DrawText(TextFormat("%f", camera.position.y), 500, 240, 20, RED);
    DrawText(TextFormat("%f", camera.position.z), 500, 260, 20, RED);
}
