#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server;
    char filename[BUF_SIZE], buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Enter filename: ");
    scanf("%s", filename);
    send(sock, filename, sizeof(filename), 0);

    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);

    if (strcmp(buffer, "File not present") == 0)
        return 0;

    while (1) {
        int choice;
        printf("\n1.Search\n2.Replace\n3.Reorder\n4.Exit\nEnter choice: ");
        scanf("%d", &choice);
        send(sock, &choice, sizeof(choice), 0);

        if (choice == 1) {
            char str[BUF_SIZE];
            printf("Enter string to search: ");
            scanf("%s", str);
            send(sock, str, sizeof(str), 0);
            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s\n", buffer);
        }

        else if (choice == 2) {
            char s1[BUF_SIZE], s2[BUF_SIZE];
            printf("Enter string to replace: ");
            scanf("%s", s1);
            printf("Enter new string: ");
            scanf("%s", s2);
            send(sock, s1, sizeof(s1), 0);
            send(sock, s2, sizeof(s2), 0);
            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s\n", buffer);
        }

        else if (choice == 3) {
            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s\n", buffer);
        }

        else if (choice == 4) {
            break;
        }
    }

    close(sock);
    return 0;
}

