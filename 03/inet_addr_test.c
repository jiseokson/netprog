#include <stdio.h>
#include <arpa/inet.h>

void print_addr(const char *addr)
{
    unsigned long conv = inet_addr(addr);
    if (conv == INADDR_NONE)
        fprintf(stderr, "Error: Invalid IPv4 address\n");
    else
        fprintf(stdout, "Network order integer: %#lx\n", conv);
}

int main(int ac, char **av)
{
    char *addr1 = "1.2.3.4";
    char *addr2 = "1.1.1.257";
    print_addr(addr1);
    print_addr(addr2);
    return 0;
}