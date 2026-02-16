#include "player.h"
#include "raylib/raylib.h"


Player InitPlayer(float yCoordinate) {
    MovementTarget currentTarget = {0};
    currentTarget.arrived = true;

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, yCoordinate, 0.0f };
    camera.target = (Vector3){ 0.0f, 0., -4.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    return (Player){
        .camera=camera,
        .currentTarget=currentTarget
    };
}


void SetPlayerTarget(Player* player, const Vector3 target) {
    player->currentTarget.destination = target;
    player->currentTarget.arrived = false;
}
