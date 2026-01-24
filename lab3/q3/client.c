#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Enter alphanumeric string: ");
    fgets(buffer, BUF_SIZE, stdin);

    send(sock, buffer, strlen(buffer), 0);

    printf("\n--- Server Responses ---\n");
    while (recv(sock, buffer, BUF_SIZE, 0) > 0) {
        printf("%s", buffer);
    }

    close(sock);
    return 0;
}

