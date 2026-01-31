#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF 1024

int main() {
    int sock, choice, book_id;
    struct sockaddr_in server;
    char buffer[BUF];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Enter Book ID to search: ");
    scanf("%d", &book_id);

    choice = 1;
    send(sock, &choice, sizeof(choice), 0);
    send(sock, &book_id, sizeof(book_id), 0);

    recv(sock, buffer, BUF, 0);
    printf("\n--- Server Response ---\n%s\n", buffer);

    close(sock);
    return 0;
}

