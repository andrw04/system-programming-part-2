#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

typedef struct {
    int server;
} args_t;

void* receive_msg(void* data) {
    args_t* args = (args_t*)data;
    char msg[200];

    while(1) {
        int size = recv(args->server, msg, 200, 0);

        if (size > 0) {
            msg[size] = '\0';
            printf("%s\n", msg);
        }

        if (size == 0) {
            printf("Connection lost\n");
            break;
        }
    }

    return NULL;
}

int main() {
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1) {
        printf("Error: invalid socket\n");
        return -1;
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(9876);
    server.sin_family = AF_INET;

    char username[11];
    printf("Enter the name: ");
    scanf("%s", username);

    printf("\nWaiting for a connection...\n");

    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) == -1) {
        printf("Error: failed connection\n");
        return -1;
    } else {
        printf("Connected\n\n");
    }

    args_t* args = (args_t*)malloc(sizeof(args_t));
    args->server = socket_desc;
    pthread_t thread;
    pthread_create(&thread, NULL, receive_msg, args);

    while(1) {
        char text[100];
        char msg[200];

        fgets(text, 100, stdin);

        if (strcmp(text, "/q") == 0) {
            break;
        }

        if(text[0] != '\n'){
            sprintf(msg, "%s: %s", username, text);
            send(socket_desc, msg, strlen(msg), 0);
        }
    }

    close(socket_desc);

    return 0;
}
