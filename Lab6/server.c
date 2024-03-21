#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

typedef struct
{
    int *sock_client;
    int *count;
    int client;
} args_t;

void *receive_send(void *data)
{
    args_t *arguments = (args_t*)data;

    char message[200];

    while(1)
    {
        int size = recv(arguments->client, message, 200, 0);

        if (size > 0)
        {
            message[size] = '\0';
            printf("%s\n", message);

            for(int i = 0; i < *arguments->count; i++)
            {
                if (arguments->sock_client[i] != arguments->client)
                {
                    send(arguments->sock_client[i], message, strlen(message), 0);
                }
            }
        }

        if (size == 0)
        {
            break;
        }
    }

    return NULL;
}

int main()
{
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0); // 0 is protocol value for IP

    if (socket_descriptor == -1)
    {
        printf("Error: invalid socket\n");
        return -1;
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(9876);
    server.sin_family = AF_INET;

    if (bind(socket_descriptor, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        printf("Error: failed connection\n");
        return -1;
    }

    listen(socket_descriptor, 10);

    char *server_address = inet_ntoa(server.sin_addr);
    int server_port = ntohs(server.sin_port);

    printf("Server started on ip-address: %s:%d\n\n", server_address, server_port);

    int *sock_client = (int*)malloc(sizeof(int));
    int count = 0;

    while (1)
    {
        struct sockaddr_in client;
        int c = sizeof(struct sockaddr_in);
        sock_client[count] = accept(socket_descriptor, (struct sockaddr *)&client, (socklen_t *)&c);
        char *client_address = inet_ntoa(client.sin_addr);
        int client_port = ntohs(client.sin_port);

        if (sock_client[count] != -1)
        {
            printf("Client connected: %s:%d\n", client_address, client_port);
            args_t *arguments = (args_t*)malloc(sizeof(args_t));
            arguments->sock_client = sock_client;
            arguments->count = &count;
            arguments->client = sock_client[count];
            pthread_t thread;
            pthread_create(&thread, NULL, receive_send, arguments);
        }

        count++;
        sock_client = (int*)realloc(sock_client, sizeof(int) * (count + 1));
    }

    return 0;
}
