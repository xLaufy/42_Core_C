#ifndef MINITALK_H
# define MINITALK_H


# include "../lib/libft/libft.h"
# include "../lib/ft_printf/ft_printf.h"

# include <stdio.h>
#include <unistd.h>
# include <signal.h>
# include <sys/types.h>

void send_char(pid_t server_pid, char c);
void handle_signals(int signum);


#endif