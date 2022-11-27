#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

void error_handling(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(1);
}

int main(int ac, char **av)
{
    int sock;
    char buffer[BUFFER_SIZE];
    int readed;
    struct sockaddr_in serv_addr;

    if (ac != 3)
    {
        fprintf(stderr, "Usage: %s <IP> <port>\n", basename(av[0]));
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(av[1]); // inet_addr : network byte order로 변환된 값 반환
    serv_addr.sin_port = htons(atoi(av[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    else
        fprintf(stdout, "Connect to server\n");

    while (1) {
        fprintf(stdout, "Input(\'Q\' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if (!strcmp(buffer, "q\n") || !strcmp(buffer, "Q\n"))
            break;

        write(sock, buffer, strlen(buffer));
        readed = read(sock, buffer, BUFFER_SIZE - 1);
        buffer[readed] = '\0';
        fprintf(stdout, "echo from server: %s", buffer);
    }

    close(sock);
    return 0;
}