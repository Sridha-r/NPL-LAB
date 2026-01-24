#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

void sort_asc(char *a, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] > a[j]) {
                char t = a[i]; a[i] = a[j]; a[j] = t;
            }
}

void sort_desc(char *a, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] < a[j]) {
                char t = a[i]; a[i] = a[j]; a[j] = t;
            }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[BUF_SIZE];

    

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
listen(server_fd, 1);

  

    printf("Server waiting for client...\n");
    client_fd = accept(server_fd, NULL, NULL);

    recv(client_fd, buffer, BUF_SIZE, 0);
    buffer[strcspn(buffer, "\n")] = '\0';

    printf("\nReceived from client: %s\n", buffer);

    char digits[BUF_SIZE], letters[BUF_SIZE];
    int d = 0, l = 0;

    for (int i = 0; buffer[i]; i++) {
        if (buffer[i] >= '0' && buffer[i] <= '9')
            digits[d++] = buffer[i];
        else if ((buffer[i] >= 'a' && buffer[i] <= 'z') ||
                 (buffer[i] >= 'A' && buffer[i] <= 'Z'))
            letters[l++] = buffer[i];
    }
    digits[d] = '\0';
    letters[l] = '\0';

    pid_t pid = fork();

    if (pid == 0) {
        /* CHILD PROCESS */
        printf("\n[Child Process]\n");
        printf("PID = %d\n", getpid());
        printf("Extracted digits: %s\n", digits);

        sort_asc(digits, strlen(digits));
        printf("Sorted digits (ascending): %s\n", digits);

        char msg[BUF_SIZE];
        sprintf(msg,
                "Child Process (PID=%d): %s\n",
                getpid(), digits);
        send(client_fd, msg, strlen(msg), 0);
    } else {
        /* PARENT PROCESS */
        printf("\n[Parent Process]\n");
        printf("PID = %d\n", getpid());
        printf("Extracted characters: %s\n", letters);

        sort_desc(letters, strlen(letters));
        printf("Sorted characters (descending): %s\n", letters);

        char msg[BUF_SIZE];
        sprintf(msg,
                "Parent Process (PID=%d): %s\n",
                getpid(), letters);
        send(client_fd, msg, strlen(msg), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

