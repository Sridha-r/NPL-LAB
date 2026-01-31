#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char domain[BUF], response[BUF];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Enter domain name: ");
    fgets(domain, BUF, stdin);

    send(sock, domain, strlen(domain), 0);

    recv(sock, response, BUF, 0);
    printf("DNS Response: %s\n", response);

    close(sock);
    return 0;
}

