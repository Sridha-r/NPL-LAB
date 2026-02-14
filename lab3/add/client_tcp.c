// tcp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9090

int main() {
    int sock = 0, choice;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Add/Subtract two integers\n");
        printf("2. Find value of x (ax + b = c)\n");
        printf("3. Multiply two matrices\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        send(sock, &choice, sizeof(choice), 0);

        if (choice == 4)
            break;

        if (choice == 1) {
            int a, b, sum, diff;
            printf("Enter two integers: ");
            scanf("%d%d", &a, &b);

            send(sock, &a, sizeof(a), 0);
            send(sock, &b, sizeof(b), 0);

            recv(sock, &sum, sizeof(sum), 0);
            recv(sock, &diff, sizeof(diff), 0);

            printf("Addition = %d\n", sum);
            printf("Subtraction = %d\n", diff);
        }

        else if (choice == 2) {
            float a, b, c, x;
            printf("For equation ax + b = c\n");
            printf("Enter a, b, c: ");
            scanf("%f%f%f", &a, &b, &c);

            send(sock, &a, sizeof(a), 0);
            send(sock, &b, sizeof(b), 0);
            send(sock, &c, sizeof(c), 0);

            recv(sock, &x, sizeof(x), 0);
            printf("Value of x = %.2f\n", x);
        }

        else if (choice == 3) {
            int r1, c1, r2, c2;
            int A[10][10], B[10][10], C[10][10];

            printf("Enter rows & cols of Matrix A: ");
            scanf("%d%d", &r1, &c1);

            printf("Enter rows & cols of Matrix B: ");
            scanf("%d%d", &r2, &c2);

            if (c1 != r2) {
                printf("Matrix multiplication not possible!\n");
                continue;
            }

            printf("Enter Matrix A:\n");
            for (int i = 0; i < r1; i++)
                for (int j = 0; j < c1; j++)
                    scanf("%d", &A[i][j]);

            printf("Enter Matrix B:\n");
            for (int i = 0; i < r2; i++)
                for (int j = 0; j < c2; j++)
                    scanf("%d", &B[i][j]);

            send(sock, &r1, sizeof(r1), 0);
            send(sock, &c1, sizeof(c1), 0);
            send(sock, &r2, sizeof(r2), 0);
            send(sock, &c2, sizeof(c2), 0);
            send(sock, A, sizeof(A), 0);
            send(sock, B, sizeof(B), 0);

            recv(sock, C, sizeof(C), 0);

            printf("Result Matrix:\n");
            for (int i = 0; i < r1; i++) {
                for (int j = 0; j < c2; j++)
                    printf("%d ", C[i][j]);
                printf("\n");
            }
        }
    }

    close(sock);
    return 0;
}
