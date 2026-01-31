#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char domain[BUF], response[BUF];
    FILE *fp;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("DNS Server waiting for client...\n");
    client_fd = accept(server_fd, NULL, NULL);

    recv(client_fd, domain, BUF, 0);
    domain[strcspn(domain, "\n")] = '\0';

    printf("Received domain request: %s\n", domain);

    fp = fopen("database.txt", "r");
    int found = 0;
    char db_domain[100], ip[100];

    if (fp != NULL) {
        while (fscanf(fp, "%s %s", db_domain, ip) != EOF) {
            if (strcmp(domain, db_domain) == 0) {
                sprintf(response, "IP Address: %s", ip);
                found = 1;
                break;
            }
        }
        fclose(fp);
    }

    if (!found) {
        strcpy(response, "Domain not found");
    }

    send(client_fd, response, strlen(response), 0);

    close(client_fd);
    close(server_fd);
    return 0;
}

