#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_SIZE 100
#define BUFFER_SIZE 1024

void sort_asc(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void sort_desc(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] < arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void split_odd_even(int arr[], int n, int odd[], int even[], int *odd_count, int *even_count) {
    *odd_count = 0;
    *even_count = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] % 2 == 0) {
            even[*even_count++] = arr[i];
        } else {
            odd[*odd_count++] = arr[i];
        }
    }
}

int search_number(int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d\n", PORT);
    
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
        
        printf("Client connected\n");
        
        while (1) {
            int n;
            read(new_socket, buffer, BUFFER_SIZE);
            
            if (strcmp(buffer, "exit") == 0) {
                printf("Client disconnected\n");
                break;
            }
            
            // Parse choice and data
            int choice, size, target;
            sscanf(buffer, "%d", &choice);
            
            // Read array size
            read(new_socket, &size, sizeof(int));
            int arr[MAX_SIZE];
            read(new_socket, arr, size * sizeof(int));
            
            char response[BUFFER_SIZE] = {0};
            
            switch (choice) {
                case 1: { // Search
                    read(new_socket, &target, sizeof(int));
                    int pos = search_number(arr, size, target);
                    sprintf(response, "Position: %d", pos);
                    break;
                }
                case 2: { // Sort ascending
                    sort_asc(arr, size);
                    sprintf(response, "Sorted Asc: ");
                    for (int i = 0; i < size; i++) {
                        strcat(response, " ");
                        char temp[10];
                        sprintf(temp, "%d", arr[i]);
                        strcat(response, temp);
                    }
                    break;
                }
                case 3: { // Sort descending
                    sort_desc(arr, size);
                    sprintf(response, "Sorted Desc: ");
                    for (int i = 0; i < size; i++) {
                        strcat(response, " ");
                        char temp[10];
                        sprintf(temp, "%d", arr[i]);
                        strcat(response, temp);
                    }
                    break;
                }
                case 4: { // Split odd/even
                    int odd[MAX_SIZE], even[MAX_SIZE];
                    int odd_count = 0, even_count = 0;
                    split_odd_even(arr, size, odd, even, &odd_count, &even_count);
                    
                    sprintf(response, "Odd(%d): ", odd_count);
                    for (int i = 0; i < odd_count; i++) {
                        char temp[10];
                        sprintf(temp, "%d ", odd[i]);
                        strcat(response, temp);
                    }
                    strcat(response, "Even(");
                    char temp[10];
                    sprintf(temp, "%d): ", even_count);
                    strcat(response, temp);
                    for (int i = 0; i < even_count; i++) {
                        sprintf(temp, "%d ", even[i]);
                        strcat(response, temp);
                    }
                    break;
                }
            }
            
            send(new_socket, response, strlen(response), 0);
            memset(buffer, 0, BUFFER_SIZE);
        }
        
        close(new_socket);
    }
    
    close(server_fd);
    return 0;
}

