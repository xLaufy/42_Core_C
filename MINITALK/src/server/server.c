#include "../../inc/minitalk.h"

void handle_signals(int signum)
{   
    static int character;
    static int bit_count;

    character = 0;
    bit_count = 0;
    if (signum == SIGUSR1)
    {
        character |= 0 << bit_count;
        bit_count++;
    }
    else if (signum == SIGUSR2)
    {
        character |= 1 << bit_count;
        bit_count++;
    }
    if (bit_count == 8)
    {
        write (1, &character, 1);
        character = 0;
        bit_count = 0;
    }
}

int main (int ac, char **av)
{
    (void)av;
    pid_t pid;

    if (ac != 1)
        ft_printf("Error\nTry: ./server\n");
    pid = getpid();
    ft_printf("PID: %d\n", pid);
    ft_printf("Waiting for message\n");
    while (ac == 1)
    {

        signal(SIGUSR1, handle_signals);
        signal(SIGUSR2, handle_signals);
        pause();
    }

    return (0);
}