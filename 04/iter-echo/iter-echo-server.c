#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int serv_sock, clnt_sock;
    char buffer[BUFFER_SIZE];
    int readed;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    if (ac != 2) {
        fprintf(stderr, "Usage: %s <port>\n", basename(av[0]));
        exit(1);
    }

    // 1. server socket 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    // 2. 주소 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(av[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 3. 설정한 주소와 socket을 연결
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    // 4. 연결 대기 (연결 대기 큐 생성)
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_addr_size = sizeof(clnt_addr);
    for (int i = 0; i < 5; ++i) {
        // 5. accept()
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
            error_handling("accept() error");
        else
            fprintf(stdout, "Connected to #%d\n", i);

        while ((readed = read(clnt_sock, buffer, BUFFER_SIZE)) > 0)
            write(clnt_sock, buffer, readed);
        close(clnt_sock);
    }

    close(serv_sock);
    return (0);
}