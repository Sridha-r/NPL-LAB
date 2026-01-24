#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

void reorder_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char text[5000];
    int n = fread(text, 1, sizeof(text), fp);
    fclose(fp);

    // Sort characters (ASCII order)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (text[i] > text[j]) {
                char temp = text[i];
                text[i] = text[j];
                text[j] = temp;
            }
        }
    }

    fp = fopen(filename, "w");
    fwrite(text, 1, n, fp);
    fclose(fp);
}

int count_occurrences(const char *filename, const char *str) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    char line[BUF_SIZE];
    int count = 0;

    while (fgets(line, sizeof(line), fp)) {
        char *pos = line;
        while ((pos = strstr(pos, str)) != NULL) {
            count++;
            pos += strlen(str);
        }
    }

    fclose(fp);
    return count;
}

int replace_string(const char *filename, const char *old, const char *new) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    char buffer[5000] = "", line[BUF_SIZE];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        char *pos;
        while ((pos = strstr(line, old)) != NULL) {
            found = 1;
            char temp[BUF_SIZE];
            strncpy(temp, line, pos - line);
            temp[pos - line] = '\0';
            strcat(buffer, temp);
            strcat(buffer, new);
            strcpy(line, pos + strlen(old));
        }
        strcat(buffer, line);
    }
    fclose(fp);

    if (found) {
        fp = fopen(filename, "w");
        fputs(buffer, fp);
        fclose(fp);
    }

    return found;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char filename[BUF_SIZE], buffer[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("Server started. Waiting for client...\n");
    client_fd = accept(server_fd, NULL, NULL);

    recv(client_fd, filename, sizeof(filename), 0);

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        send(client_fd, "File not present", 16, 0);
        close(client_fd);
        close(server_fd);
        return 0;
    }
    fclose(fp);
    send(client_fd, "File present", 12, 0);

    while (1) {
        int choice;
        recv(client_fd, &choice, sizeof(choice), 0);

        if (choice == 1) {
            char search[BUF_SIZE];
            recv(client_fd, search, sizeof(search), 0);

            int count = count_occurrences(filename, search);
            if (count > 0)
                sprintf(buffer, "String found %d times", count);
            else
                strcpy(buffer, "String not found");

            send(client_fd, buffer, sizeof(buffer), 0);
        }

        else if (choice == 2) {
            char s1[BUF_SIZE], s2[BUF_SIZE];
            recv(client_fd, s1, sizeof(s1), 0);
            recv(client_fd, s2, sizeof(s2), 0);

            if (replace_string(filename, s1, s2))
                send(client_fd, "String replaced", 15, 0);
            else
                send(client_fd, "String not found", 16, 0);
        }

        else if (choice == 3) {
            reorder_file(filename);
            send(client_fd, "File reordered", 14, 0);
        }

        else if (choice == 4) {
            break;
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

