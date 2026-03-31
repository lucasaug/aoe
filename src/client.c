#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>


#include "raylib/raylib.h"

#include "scene.h"
#include "world.h"

#define PORT "1235"

int connect_to_server() {
    struct addrinfo hints, *res;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo("127.0.0.1", PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -2;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        fprintf(stderr, "error creating socket: %d\n", errno);
        return -3;
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        fprintf(stderr, "error connecting: %d\n", errno);
        return -4;
    }

    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    return sockfd;
}


int main(void) {
    int sockfd = connect_to_server();
    int received_value = -1;
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "My weird game");

    Scene scene = {0};
    InitScene(&scene, screenWidth, screenHeight, MAX_HEIGHT + 10.);

    DisableCursor();
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose()) {
        int newval;
        int result = recv(sockfd, &newval, sizeof(int), 0);
        if (result != -1) {
            received_value = newval;
        } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
            printf("Error! recv\n");
            break;
        }
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
