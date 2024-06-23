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

#include "../inc/minitalk.h"

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
			send_char(pid, *message);
			message++;
		}
		send_char(pid, '\n');
	}
	else
	{
		write(1, "Error\nTry: ./client [server_pid] [message]\n", 43);
		return (1);
	}
	return (0);
}
