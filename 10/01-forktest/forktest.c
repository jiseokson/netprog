#include <stdio.h>
#include <unistd.h>

int gval = 10;

int main(int ac, char **av)
{
    pid_t pid;
    int lval = 20;
    ++gval, lval += 5;

    printf("%d, %d\n", gval, lval); // 11, 25
    pid = fork();
    if (pid == 0)
        gval += 2, lval += 2;
    else
        gval -= 2, lval -= 2;
    printf("%s: %d, %d\n", (pid == 0 ? "Child proc" : "Parent proc"), gval, lval);
    
    return 0;
}
