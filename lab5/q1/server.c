#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 2

int client_count = 0;
int client_sockets[10];
struct sockaddr_in client_addresses[10];

pthread_mutex_t lock;

void *handle_client(void *arg)
{
    int sock = *(int *)arg;
    char buffer[1024];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    memset(buffer, 0, sizeof(buffer));

    getpeername(sock, (struct sockaddr *)&addr, &addr_len);

    recv(sock, buffer, sizeof(buffer), 0);

    printf("Received from %s:%d -> %s\n",
           inet_ntoa(addr.sin_addr),
           ntohs(addr.sin_port),
           buffer);

    close(sock);
    return NULL;
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t tid;

    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

        pthread_mutex_lock(&lock);

        if (client_count >= MAX_CLIENTS)
        {
            for (int i = 0; i < client_count; i++)
            {
                send(client_sockets[i], "terminate session", 17, 0);
                close(client_sockets[i]);
            }

            send(new_socket, "terminate session", 17, 0);
            close(new_socket);

            printf("More than 2 clients connected. Server terminating.\n");
            exit(0);
        }

        client_sockets[client_count] = new_socket;
        client_addresses[client_count] = client_addr;
        client_count++;

        pthread_mutex_unlock(&lock);

        pthread_create(&tid, NULL, handle_client, &new_socket);
        pthread_detach(tid);

        if (client_count == 2)
        {
            FILE *fp = fopen("data.txt", "r");
            char word[100];

            if (fp == NULL)
            {
                perror("File open error");
                exit(1);
            }

            fscanf(fp, "%s", word);
            fclose(fp);

            printf("\nFinal Output:\n");
            printf("%s Institute of Technology\n", word);

            printf("Client 1 Address: %s:%d\n",
                   inet_ntoa(client_addresses[0].sin_addr),
                   ntohs(client_addresses[0].sin_port));

            printf("Client 2 Address: %s:%d\n",
                   inet_ntoa(client_addresses[1].sin_addr),
                   ntohs(client_addresses[1].sin_port));
        }
    }

    close(server_fd);
    return 0;
}

