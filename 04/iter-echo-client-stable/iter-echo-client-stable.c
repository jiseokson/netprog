#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

void error_handling(const char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}

int main(int ac, char **av)
{
    int sock;
    struct sockaddr_in server_address;

    if (ac != 3) {
        fprintf(stderr, "Usage: %s <IP> <port>\n", av[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(av[1]);
    server_address.sin_port = htons(atoi(av[2]));

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
        error_handling("connect() error");
    else
        fprintf(stdout, "Connect to server\n");

    while (1) {
        char buffer[BUFFER_SIZE];
        ssize_t received = 0;
        
        fprintf(stdout, "Input(\'Q\' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if (!strncmp(buffer, "q\n", 2) || !strncmp(buffer, "Q\n", 2))
            break;

        ssize_t len = write(sock, buffer, strlen(buffer));
        while (received < len) {
            ssize_t readed = read(sock, buffer + received, BUFFER_SIZE - 1);
            if (readed < -1)
                error_handling("read() error");
            buffer[readed] = '\0';
            received += readed;
        }
        fprintf(stdout, "echo from server: %s", buffer);
    }

    close(sock);
    return 0;
}