#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define BUF 1024

struct Library {
    int book_id;
    char book_name[50];
    char author[50];
    int available;
};

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[BUF];
    int choice;

    struct Library db[] = {
        {101, "CN", "Tanenbaum", 1},
        {102, "OS", "Silberschatz", 0},
        {103, "DBMS", "Korth", 1}
    };
    int n = 3;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("Library Server waiting...\n");
    client_fd = accept(server_fd, NULL, NULL);

    recv(client_fd, &choice, sizeof(choice), 0);

    pid_t pid = fork();

    if (pid == 0) {   // CHILD
        char response[BUF] = "";
        int book_id;

        recv(client_fd, &book_id, sizeof(book_id), 0);

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
            sprintf(response, "PID=%d\nBook not found\n", getpid());
        }

        send(client_fd, response, strlen(response), 0);
        exit(0);
    }

    wait(NULL);
    close(client_fd);
    close(server_fd);
    return 0;
}

