#include <stdio.h>
#include <arpa/inet.h>

int main(int ac, char **av)
{
    char *addr_str = "127.222.123.4";
    struct sockaddr_in addr;

    if (!inet_aton(addr_str, &addr.sin_addr))
        fprintf(stderr, "Invalid IPv4 address\n");
    else
        fprintf(stdout, "Network ordered address: %#x\n", addr.sin_addr.s_addr);
    return 0;
}