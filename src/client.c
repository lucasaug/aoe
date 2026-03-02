#include <errno.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "raylib/raylib.h"

#include "scene.h"
#include "world.h"

#define PORT "1234"


int main(void) {
    int received_value = -1;
    if (!fork()) {
        struct addrinfo hints, *res;
        int status;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if ((status = getaddrinfo("127.0.0.1", PORT, &hints, &res)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
            return 2;
        }

        int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd == -1) {
            fprintf(stderr, "error creating socket: %d\n", errno);
            return 3;
        }

        if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
            fprintf(stderr, "error connecting: %d\n", errno);
            return 4;
        }

        while (recv(sockfd, &received_value, sizeof(int), 0)) {}
    }

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
        scene.value = received_value;
        RenderScene(&scene);
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    FreeScene(&scene);
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}
