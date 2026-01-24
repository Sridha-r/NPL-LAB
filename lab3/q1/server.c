#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    pid_t pid;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);

    printf("Server waiting for connection...\n");
    client_fd = accept(server_fd, NULL, NULL);

    printf("\n[SERVER PARENT] PID=%d PPID=%d\n", getpid(), getppid());

    pid = fork();

    if (pid == 0) {
        /* CHILD PROCESS → Receive messages */
        printf("[SERVER CHILD] PID=%d PPID=%d\n", getpid(), getppid());
        while (1) {
            memset(buffer, 0, BUF_SIZE);
            int n = recv(client_fd, buffer, BUF_SIZE - 1, 0);

if (n <= 0) {
    printf("Client disconnected\n");
    break;
}

buffer[n] = '\0';
printf("Client: %s", buffer);
fflush(stdout);



buffer[n] = '\0';
printf("Server: %s", buffer);
fflush(stdout);

        }
    } else {
        /* PARENT PROCESS → Send messages */
        while (1) {
            memset(buffer, 0, BUF_SIZE);
            fgets(buffer, BUF_SIZE, stdin);
            send(client_fd, buffer, strlen(buffer), 0);
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

