#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char str1[100], str2[100], result[100];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Enter first string: ");
    scanf("%s", str1);

    printf("Enter second string: ");
    scanf("%s", str2);

    send(sock, str1, strlen(str1), 0);
    send(sock, str2, strlen(str2), 0);

    memset(result, 0, sizeof(result));
    recv(sock, result, sizeof(result), 0);

    printf("\nServer Response: %s\n", result);

    close(sock);
    return 0;
}

