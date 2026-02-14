// udp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 4096

void analyze_file(FILE *fp, int *alphabets, int *digits, int *spaces,
                  int *lines, int *others, int *size) {
    char ch;
    *alphabets = *digits = *spaces = *lines = *others = *size = 0;

    while ((ch = fgetc(fp)) != EOF) {
        (*size)++;
        if (isalpha(ch))
            (*alphabets)++;
        else if (isdigit(ch))
            (*digits)++;
        else if (isspace(ch)) {
            (*spaces)++;
            if (ch == '\n')
                (*lines)++;
        } else
            (*others)++;
    }
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("UDP File Server running on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                 (struct sockaddr*)&client_addr, &addr_len);

        if (strcmp(buffer, "stop") == 0)
            break;

        FILE *fp = fopen(buffer, "r");

        if (fp == NULL) {
            strcpy(buffer, "ERROR: File not found.");
            sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr*)&client_addr, addr_len);
        } else {
            int alphabets, digits, spaces, lines, others, size;
            analyze_file(fp, &alphabets, &digits, &spaces,
                         &lines, &others, &size);

            rewind(fp);

            char file_content[BUFFER_SIZE] = "";
            fread(file_content, sizeof(char), BUFFER_SIZE, fp);

            char result[BUFFER_SIZE];
            snprintf(result, BUFFER_SIZE,
                     "File Content:\n%s\n\nFile Size: %d bytes\nAlphabets: %d\nDigits: %d\nSpaces: %d\nLines: %d\nOther Characters: %d\n",
                     file_content, size, alphabets, digits,
                     spaces, lines, others);

            sendto(sockfd, result, strlen(result), 0,
                   (struct sockaddr*)&client_addr, addr_len);

            fclose(fp);
        }
    }

    close(sockfd);
    return 0;
}
