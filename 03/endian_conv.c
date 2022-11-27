#include <stdio.h>
#include <arpa/inet.h>

int main(int ac, char **av)
{
    unsigned short host_port = 0x1234;
    unsigned short net_port = htons(host_port);
    unsigned long host_addr = 0x12345678;
    unsigned long net_addr = htonl(host_addr);

    printf("host port: %#x\n", host_port);
    printf("net port: %#x\n", net_port);
    printf("host addr: %#lx\n", host_addr);
    printf("net addr: %#lx\n", net_addr);
    return 0;
}