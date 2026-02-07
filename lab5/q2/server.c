#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080

int areAnagrams(char str1[], char str2[])
{
    int count[256] = {0};

    for (int i = 0; str1[i] && str2[i]; i++)
    {
        count[(unsigned char)str1[i]]++;
        count[(unsigned char)str2[i]]--;
    }

    for (int i = 0; i < 256; i++)
    {
        if (count[i] != 0)
            return 0;
    }
    return 1;
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char str1[100], str2[100], result[100];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Iterative Server listening on port %d...\n", PORT);

    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

        memset(str1, 0, sizeof(str1));
        memset(str2, 0, sizeof(str2));

        recv(client_fd, str1, sizeof(str1), 0);
        recv(client_fd, str2, sizeof(str2), 0);

        time_t now;
        time(&now);

        printf("\nConnected Client Address: %s:%d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        printf("Date & Time: %s", ctime(&now));

        if (strlen(str1) == strlen(str2) && areAnagrams(str1, str2))
            strcpy(result, "Strings are Anagrams");
        else
            strcpy(result, "Strings are NOT Anagrams");

        send(client_fd, result, strlen(result), 0);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}

