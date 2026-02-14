// tcp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9090
#define MAX 100

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("TCP Calculator Server Running on port %d...\n", PORT);

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    while (1) {
        int choice;
        recv(new_socket, &choice, sizeof(choice), 0);

        if (choice == 4)
            break;

        if (choice == 1) {
            int a, b;
            recv(new_socket, &a, sizeof(a), 0);
            recv(new_socket, &b, sizeof(b), 0);

            int sum = a + b;
            int diff = a - b;

            send(new_socket, &sum, sizeof(sum), 0);
            send(new_socket, &diff, sizeof(diff), 0);
        }

        else if (choice == 2) {
            // ax + b = c → x = (c - b) / a
            float a, b, c;
            recv(new_socket, &a, sizeof(a), 0);
            recv(new_socket, &b, sizeof(b), 0);
            recv(new_socket, &c, sizeof(c), 0);

            float x = (c - b) / a;
            send(new_socket, &x, sizeof(x), 0);
        }

        else if (choice == 3) {
            int r1, c1, r2, c2;
            recv(new_socket, &r1, sizeof(r1), 0);
            recv(new_socket, &c1, sizeof(c1), 0);
            recv(new_socket, &r2, sizeof(r2), 0);
            recv(new_socket, &c2, sizeof(c2), 0);

            int A[10][10], B[10][10], C[10][10];

            recv(new_socket, A, sizeof(A), 0);
            recv(new_socket, B, sizeof(B), 0);

            for (int i = 0; i < r1; i++) {
                for (int j = 0; j < c2; j++) {
                    C[i][j] = 0;
                    for (int k = 0; k < c1; k++)
                        C[i][j] += A[i][k] * B[k][j];
                }
            }

            send(new_socket, C, sizeof(C), 0);
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
