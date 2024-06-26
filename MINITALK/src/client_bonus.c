/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/06/07 16:10:49 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/06/07 16:10:49 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk_bonus.h"

static void	sig_confirm(int sig)
{
	if (sig == SIGUSR1)
		ft_printf("Got the message Bruh\n");
	else
		ft_printf("Got the message Bruh\n");
}

void	send_char(pid_t server_pid, char c)
{
	int	bit_count;

	bit_count = 0;
	while (bit_count < 8)
	{
		if ((c & (1 << bit_count)) != 0)
			kill(server_pid, SIGUSR1);
		else
			kill(server_pid, SIGUSR2);
		usleep(100);
		bit_count++;
	}
}

int	main(int ac, char **av)
{
	pid_t	pid;
	char	*message;

	if (ac == 3)
	{
		pid = ft_atoi(av[1]);
		message = av[2];
		while (*message != '\0')
		{
			signal(SIGUSR1, sig_confirm);
			signal(SIGUSR2, sig_confirm);
			send_char(pid, *message);
			message++;
		}
		send_char(pid, '\n');
	}
	else
	{
		ft_printf("Error\nTry: ./client [server_pid] [message]\n");
		return (1);
	}
	return (0);
}
