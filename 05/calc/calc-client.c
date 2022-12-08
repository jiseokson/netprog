#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    if (ac != 3) {
        printf("Usage: %s <IP address> <port>\n", av[0]);
        exit(1);
    }

    int client_socket = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = PF_INET;
    server_address.sin_addr.s_addr = inet_addr(av[1]);
    server_address.sin_port = htons(atoi(av[2]));

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
        error_handling("connect() error");
    else
        printf("Connection success!\n");

    char operartor;
    char operand_cnt;
    printf("operator: ");
    scanf("%c", &operartor);
    printf("operand cnt: ");
    scanf("%hhd", &operand_cnt);
    write(client_socket, &operartor, 1);
    write(client_socket, &operand_cnt, 1);

    for (int i = 0; i < operand_cnt; ++i) {
        int n;
        printf("num %d: ", i);
        scanf("%d", &n);
        write(client_socket, &n, sizeof(int));
    }

    ssize_t received = 0;
    ssize_t readed = 0;
    char buffer[BUFFER_SIZE];
    while (received < sizeof(int)) {
        readed = read(client_socket, buffer + received, BUFFER_SIZE - received);
        if (readed == -1)
            error_handling("read() error");
        received += readed;
    }

    int res = *(int *)&buffer;
    printf("Result from calc server: %d\n", res);

    close(client_socket);
    return 0;
}