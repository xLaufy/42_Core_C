/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/06/07 16:10:52 by mkrawczy          #+#    #+#             */
/*   Updated: 2024/06/07 16:10:52 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minitalk.h"

void	handle_signals(int signum)
{
	static int	character;
	static int	bit_count;

	if (signum == SIGUSR1)
	{
		character |= (1 << bit_count);
		bit_count++;
	}
	else if (signum == SIGUSR2)
	{
		character |= (0 << bit_count);
		bit_count++;
	}
	if (bit_count == 8)
	{
		ft_printf("%c", character);
		bit_count = 0;
		character = 0;
	}
}

int	main(int ac, char **av)
{
	pid_t	pid;

	(void)av;
	if (ac != 1)
		ft_printf("Error\nTry: ./server\n");
	pid = getpid();
	ft_printf("PID: %d\n", pid);
	ft_printf("Waiting for message...\n");
	while (1)
	{
		signal(SIGUSR1, handle_signals);
		signal(SIGUSR2, handle_signals);
		pause();
	}
	return (0);
}
