#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>


#include "messages.h"
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


int handle_message(Scene* scene, enum MessageType type, void* msg) {
    if (type == PLAYER_POSITION) {
        PlayerPosition* playerPosMsg = (PlayerPosition*) msg;
        scene->otherPlayerPos = playerPosMsg->position;

        return 0;
    }

    return -1;
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

    unsigned int id = GetRandomValue(0, 500);

    while (!WindowShouldClose()) {
        enum MessageType type;
        int result = recv(sockfd, &type, sizeof(enum MessageType), 0);
        if (result != -1) {
            void* msg = recvMessage(sockfd, type);

            if (msg == NULL) {
                printf("Error! recvMessage result is NULL");
                break;
            }

            int handlerResult = handle_message(&scene, type, msg);
            if (handlerResult == -1) {
                printf("Error! handle_message: %d\n", handlerResult);
                break;
            }
        } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
            printf("Error! recv: %d\n", errno);
            break;
        }

        PlayerPosition msg = (PlayerPosition) {
            scene.player.camera.position,
            id
        };
        sendMessage(sockfd, PLAYER_POSITION, &msg);

        UpdateScene(&scene);
        scene.value = received_value;
        RenderScene(&scene);
    }

    FreeScene(&scene);
    CloseWindow();

    return 0;
}
