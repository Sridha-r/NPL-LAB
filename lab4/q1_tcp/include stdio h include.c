#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define BUF 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[BUF];
    int option;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("TCP Server waiting...\n");
    client_fd = accept(server_fd, NULL, NULL);

    recv(client_fd, &option, sizeof(option), 0);
    recv(client_fd, buffer, BUF, 0);

    printf("\n[Parent] Received option %d, assigning task...\n", option);

    pid_t pid = fork();

    if (pid == 0) {   // CHILD PROCESS
        char response[BUF];

        if (option == 1) {
            printf("[Child-1] PID=%d Processing Registration Number\n", getpid());
            sprintf(response,
                "PID=%d\nName: Rahul Sharma\nAddress: Bangalore\n",
                getpid());
        }
        else if (option == 2) {
            printf("[Child-2] PID=%d Processing Student Name\n", getpid());
            sprintf(response,
                "PID=%d\nDept: CSE\nSemester: 5\nSection: B\nCourses: CN, OS, DBMS\n",
                getpid());
        }
        else if (option == 3) {
            printf("[Child-3] PID=%d Processing Subject Code\n", getpid());
            sprintf(response,
                "PID=%d\nMarks Obtained: 87\n",
                getpid());
        }

        send(client_fd, response, strlen(response), 0);
        exit(0);
    }

    wait(NULL);
    close(client_fd);
    close(server_fd);
    return 0;
}

