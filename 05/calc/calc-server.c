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
    if (ac != 2) {
        printf("Usage: %s <port>\n", av[0]);
        exit(1);
    }

    int server_socket;
    struct sockaddr_in server_address;

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
        error_handling("socket() error");

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(av[1]));
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
        error_handling("bind() error");

    if (listen(server_socket, 5) == -1)
        error_handling("listen() error");

    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);

    int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
    if (client_socket == -1)
        error_handling("accpet() error");
    else
        printf("Connection success!\n");

    ssize_t received = 0;
    ssize_t readed = 0;

    char operator;
    char operand_cnt = 0;
    unsigned char buffer[BUFFER_SIZE];

    while (received < 1) {
        readed = read(client_socket, &operator, 1);
        if (readed == -1)
            error_handling("read() error: get operator");
        received += readed;
    }
    while (received < 2) {
        readed = read(client_socket, &operand_cnt, 1);
        if (readed == -1)
            error_handling("read() error: get operand cnt");
        received += readed;
    }

    received = 0;
    while (received < operand_cnt * sizeof(int)) {
        readed = read(client_socket, buffer + received, BUFFER_SIZE - received);
        if (readed == -1)
            error_handling("read() error: get operands");
        received += readed;
    }

    // for debug
    printf("operator: %c, operand cnt: %d\n", operator, operand_cnt);
    printf("num stream\n");
    for (int i = 0; i < received; ++i)
        printf("%.2x ", buffer[i]);
    printf("\n");

    int *nums = (int *)buffer;
    for (int i = 0; i < operand_cnt; ++i)
        printf("nums[%d]: %d\n", i, nums[i]);
    int ret = nums[0];
    for (int i = 1; i < operand_cnt; ++i) {
        switch (operator) {
        case '+':
            ret += nums[i];
            break;
        case '-':
            ret -= nums[i];
            break;
        default:
            ret = 0;
            break;
        }
    }
    printf("res: %d\n", ret);
    write(client_socket, &ret, sizeof(int));

    close(client_socket);
    close(server_socket);
    return 0;
}