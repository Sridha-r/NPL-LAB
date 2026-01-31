#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF 1024

struct Library {
    int book_id;
    char book_name[50];
    char author[50];
    int available;
};

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);
    char buffer[BUF];
    int book_id;

    struct Library db[] = {
        {101, "CN", "Tanenbaum", 1},
        {102, "OS", "Silberschatz", 0},
        {103, "DBMS", "Korth", 1}
    };
    int n = 3;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("UDP Library Server waiting...\n");

    recvfrom(sockfd, &book_id, sizeof(book_id), 0,
             (struct sockaddr *)&client_addr, &len);

    printf("[Parent] Received Book ID: %d\n", book_id);

    pid_t pid = fork();

    if (pid == 0) {   // CHILD PROCESS
        char response[BUF] = "";
        printf("[Child PID=%d] Searching database...\n", getpid());

        for (int i = 0; i < n; i++) {
            if (db[i].book_id == book_id) {
                sprintf(response,
                    "PID=%d\nBook: %s\nAuthor: %s\nAvailable: %s\n",
                    getpid(),
                    db[i].book_name,
                    db[i].author,
                    db[i].available ? "Yes" : "No");
                break;
            }
        }

        if (strlen(response) == 0) {
            sprintf(response,
                "PID=%d\nBook not found\n",
                getpid());
        }

        sendto(sockfd, response, strlen(response), 0,
               (struct sockaddr *)&client_addr, len);
        exit(0);
    }

    close(sockfd);
    return 0;
}

