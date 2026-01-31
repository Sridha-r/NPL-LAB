#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF 1024

int main() {
    int sockfd, book_id;
    struct sockaddr_in server_addr;
    char buffer[BUF];
    socklen_t len = sizeof(server_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter Book ID to search: ");
    scanf("%d", &book_id);

    sendto(sockfd, &book_id, sizeof(book_id), 0,
           (struct sockaddr *)&server_addr, len);

    recvfrom(sockfd, buffer, BUF, 0,
             (struct sockaddr *)&server_addr, &len);

    printf("\n--- Server Response ---\n%s\n", buffer);

    close(sockfd);
    return 0;
}

