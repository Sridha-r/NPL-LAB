// travel_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 8085

int total_seats[2] = {50, 40};
int booked_seats[2] = {0, 0};

void handle_client(int new_socket) {
    int route, seats;

    // Display seat status
    printf("\n--- Current Seat Status ---\n");
    printf("1. Chennai → Bangalore | Available: %d | Booked: %d\n",
           total_seats[0] - booked_seats[0], booked_seats[0]);
    printf("2. Mumbai → Delhi      | Available: %d | Booked: %d\n",
           total_seats[1] - booked_seats[1], booked_seats[1]);

    recv(new_socket, &route, sizeof(route), 0);
    recv(new_socket, &seats, sizeof(seats), 0);

    int index = route - 1;
    char message[100];

    if (index < 0 || index > 1) {
        strcpy(message, "Invalid Route Selected!");
    }
    else if ((total_seats[index] - booked_seats[index]) >= seats) {
        booked_seats[index] += seats;
        strcpy(message, "Seat Reservation Successful!");
    }
    else {
        strcpy(message, "Requested seats unavailable!");
    }

    send(new_socket, message, sizeof(message), 0);
    close(new_socket);
    exit(0);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("Travel Reservation Server Running on port %d...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (fork() == 0) {   // Child process handles client
            close(server_fd);
            handle_client(new_socket);
        }

        close(new_socket);
    }

    return 0;
}
