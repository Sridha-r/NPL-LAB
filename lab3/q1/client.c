#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    pid_t pid;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("\n[CLIENT PARENT] PID=%d PPID=%d\n", getpid(), getppid());

    pid = fork();

    if (pid == 0) {
        /* CHILD PROCESS → Receive messages */
        printf("[CLIENT CHILD] PID=%d PPID=%d\n", getpid(), getppid());
        while (1) {
            memset(buffer, 0, BUF_SIZE);
           int n = recv(sock, buffer, BUF_SIZE - 1, 0);

if (n <= 0) {
    printf("Server disconnected\n");
    break;
}

buffer[n] = '\0';
printf("Server: %s", buffer);
fflush(stdout);

        }
    } else {
        /* PARENT PROCESS → Send messages */
        while (1) {
            memset(buffer, 0, BUF_SIZE);
            fgets(buffer, BUF_SIZE, stdin);
            send(sock, buffer, strlen(buffer), 0);
        }
    }

    close(sock);
    return 0;
}

