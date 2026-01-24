#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

void permute(char *str, int l, int r) {
    if (l == r) {
        printf("%s\n", str);
        return;
    }
    for (int i = l; i <= r; i++) {
        swap(&str[l], &str[i]);
        permute(str, l + 1, r);
        swap(&str[l], &str[i]); // backtrack
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE];
    socklen_t len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("UDP Server waiting for client message...\n");

    int n = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0,
                 (struct sockaddr *)&client_addr, &len);

buffer[n] = '\0';
printf("Received string: %s\n", buffer);


    buffer[strcspn(buffer, "\n")] = '\0';

    printf("\nReceived string: %s\n", buffer);
    printf("Permutations:\n");

    permute(buffer, 0, strlen(buffer) - 1);

    close(sockfd);
    return 0;
}

