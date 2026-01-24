#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_SIZE 100
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation failed\n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("Invalid address\n");
        return -1;
    }
    
    while (1) {
        printf("\n--- Client Menu ---\n");
        printf("1. Search number\n");
        printf("2. Sort Ascending\n");
        printf("3. Sort Descending\n");
        printf("4. Split Odd/Even\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        
        int choice;
        scanf("%d", &choice);
        
        if (choice == 5) {
            send(sock, "exit", 4, 0);
            break;
        }
        
        // Connect to server for each operation
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("Connection Failed\n");
            continue;
        }
        
        // Send choice
        sprintf(buffer, "%d", choice);
        send(sock, buffer, strlen(buffer), 0);
        
        // Input array
        int size;
        printf("Enter number of elements: ");
        scanf("%d", &size);
        int arr[MAX_SIZE];
        printf("Enter %d integers: ", size);
        for (int i = 0; i < size; i++) {
            scanf("%d", &arr[i]);
        }
        
        // Send array size and data
        send(sock, &size, sizeof(int), 0);
        send(sock, arr, size * sizeof(int), 0);
        
        if (choice == 1) {
            int target;
            printf("Enter number to search: ");
            scanf("%d", &target);
            send(sock, &target, sizeof(int), 0);
        }
        
        // Receive result
        read(sock, buffer, BUFFER_SIZE);
        printf("Server response: %s\n", buffer);
        
        close(sock);
        sock = socket(AF_INET, SOCK_STREAM, 0); // Create new socket for next iteration
    }
    
    return 0;
}

