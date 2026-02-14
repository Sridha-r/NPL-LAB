// book_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 7070
#define MAX 100

typedef struct {
    char title[50];
    char author[50];
    int accession_no;
    int pages;
    char publisher[50];
} Book;

Book library[MAX];
int count = 0;

int main() {
    int server_fd, new_socket, choice;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Book Database Server Running...\n");

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    while (1) {
        recv(new_socket, &choice, sizeof(choice), 0);

        if (choice == 5)
            break;

        // INSERT
        if (choice == 1) {
            Book b;
            recv(new_socket, &b, sizeof(Book), 0);

            library[count++] = b;
            char msg[] = "Book inserted successfully!";
            send(new_socket, msg, sizeof(msg), 0);
        }

        // DELETE
        else if (choice == 2) {
            int acc, found = 0;
            recv(new_socket, &acc, sizeof(acc), 0);

            for (int i = 0; i < count; i++) {
                if (library[i].accession_no == acc) {
                    for (int j = i; j < count - 1; j++)
                        library[j] = library[j + 1];
                    count--;
                    found = 1;
                    break;
                }
            }

            if (found)
                send(new_socket, "Book deleted successfully!", 30, 0);
            else
                send(new_socket, "Book not found!", 20, 0);
        }

        // DISPLAY ALL
        else if (choice == 3) {
            send(new_socket, &count, sizeof(count), 0);
            send(new_socket, library, sizeof(Book) * count, 0);
        }

        // SEARCH
        else if (choice == 4) {
            int search_type;
            recv(new_socket, &search_type, sizeof(search_type), 0);

            if (search_type == 1) { // Title
                char title[50];
                recv(new_socket, title, sizeof(title), 0);

                int found = 0;
                for (int i = 0; i < count; i++) {
                    if (strcmp(library[i].title, title) == 0) {
                        send(new_socket, &library[i], sizeof(Book), 0);
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    Book empty = {"NOTFOUND", "", 0, 0, ""};
                    send(new_socket, &empty, sizeof(Book), 0);
                }
            }

            else if (search_type == 2) { // Author
                char author[50];
                recv(new_socket, author, sizeof(author), 0);

                int match_count = 0;
                for (int i = 0; i < count; i++)
                    if (strcmp(library[i].author, author) == 0)
                        match_count++;

                send(new_socket, &match_count, sizeof(match_count), 0);

                for (int i = 0; i < count; i++)
                    if (strcmp(library[i].author, author) == 0)
                        send(new_socket, &library[i], sizeof(Book), 0);
            }
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
