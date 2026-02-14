// travel_client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8085

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    int route, seats;
    char message[100];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("\n--- Travel Reservation ---\n");
    printf("1. Chennai → Bangalore\n");
    printf("2. Mumbai → Delhi\n");
    printf("Select Route (1/2): ");
    scanf("%d", &route);

    printf("Enter number of seats to book: ");
    scanf("%d", &seats);

    send(sock, &route, sizeof(route), 0);
    send(sock, &seats, sizeof(seats), 0);

    recv(sock, message, sizeof(message), 0);

    printf("\nServer Response: %s\n", message);

    close(sock);
    return 0;
}
