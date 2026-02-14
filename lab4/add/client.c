// book_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 7070

typedef struct {
    char title[50];
    char author[50];
    int accession_no;
    int pages;
    char publisher[50];
} Book;

int main() {
    int sock = 0, choice;
    struct sockaddr_in serv_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1) {
        printf("\n--- BOOK DATABASE MENU ---\n");
        printf("1. Insert Book\n");
        printf("2. Delete Book\n");
        printf("3. Display All Books\n");
        printf("4. Search Book\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        send(sock, &choice, sizeof(choice), 0);

        if (choice == 5)
            break;

        if (choice == 1) {
            Book b;
            printf("Enter Title: ");
            scanf(" %[^\n]", b.title);
            printf("Enter Author: ");
            scanf(" %[^\n]", b.author);
            printf("Enter Accession No: ");
            scanf("%d", &b.accession_no);
            printf("Enter Pages: ");
            scanf("%d", &b.pages);
            printf("Enter Publisher: ");
            scanf(" %[^\n]", b.publisher);

            send(sock, &b, sizeof(Book), 0);

            char msg[100];
            recv(sock, msg, sizeof(msg), 0);
            printf("%s\n", msg);
        }

        else if (choice == 2) {
            int acc;
            printf("Enter Accession No to delete: ");
            scanf("%d", &acc);
            send(sock, &acc, sizeof(acc), 0);

            char msg[100];
            recv(sock, msg, sizeof(msg), 0);
            printf("%s\n", msg);
        }

        else if (choice == 3) {
            int count;
            recv(sock, &count, sizeof(count), 0);

            Book books[100];
            recv(sock, books, sizeof(Book) * count, 0);

            for (int i = 0; i < count; i++) {
                printf("\nTitle: %s\nAuthor: %s\nAcc No: %d\nPages: %d\nPublisher: %s\n",
                       books[i].title, books[i].author,
                       books[i].accession_no,
                       books[i].pages,
                       books[i].publisher);
            }
        }

        else if (choice == 4) {
            int type;
            printf("1. Search by Title\n2. Search by Author\nEnter: ");
            scanf("%d", &type);

            send(sock, &type, sizeof(type), 0);

            if (type == 1) {
                char title[50];
                printf("Enter Title: ");
                scanf(" %[^\n]", title);
                send(sock, title, sizeof(title), 0);

                Book b;
                recv(sock, &b, sizeof(Book), 0);

                if (strcmp(b.title, "NOTFOUND") == 0)
                    printf("Book not found!\n");
                else
                    printf("\nTitle: %s\nAuthor: %s\nAcc No: %d\nPages: %d\nPublisher: %s\n",
                           b.title, b.author, b.accession_no, b.pages, b.publisher);
            }

            else if (type == 2) {
                char author[50];
                printf("Enter Author: ");
                scanf(" %[^\n]", author);
                send(sock, author, sizeof(author), 0);

                int match_count;
                recv(sock, &match_count, sizeof(match_count), 0);

                if (match_count == 0)
                    printf("No books found for this author!\n");
                else {
                    for (int i = 0; i < match_count; i++) {
                        Book b;
                        recv(sock, &b, sizeof(Book), 0);
                        printf("\nTitle: %s\nAcc No: %d\nPages: %d\nPublisher: %s\n",
                               b.title, b.accession_no, b.pages, b.publisher);
                    }
                }
            }
        }
    }

    close(sock);
    return 0;
}
