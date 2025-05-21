<<<<<<< HEAD
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 20:19:34 by hhurnik           #+#    #+#             */
/*   Updated: 2025/04/28 19:07:24 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
#include "../inc/minishell.h"

void	write_stderr(char *str)
{
	write(2, str, ft_strlen(str));
}

<<<<<<< HEAD
int is_integer(char *str)
{
	int i = 0;

	if (!str || !*str)
		return (0);

	// Optional sign
	if (str[i] == '-' || str[i] == '+')
		i++;

	// At least one digit required
	if (!ft_isdigit(str[i]))
		return (0);

	// All remaining must be digits
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
=======
int	is_integer(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
			return (0);
		i++;
	}
	return (1);
}

<<<<<<< HEAD

int parse_exit_args(char *argv[], int *exit_code)
{
	char *str = NULL;

=======
int	parse_exit_args(char *argv[], int *exit_code)
{
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
	if (argv[1] && argv[2])
	{
		write_stderr("exit: too many arguments\n");
		*exit_code = 1;
<<<<<<< HEAD
		return (1); // Don't exit
	}

	if (argv[1])
	{
		// Allow quoted strings
		if (argv[1][0] == '"' || argv[1][0] == '\'')
			str = ft_strtrim(argv[1], "\"'");
		else
			str = ft_strdup(argv[1]);

		if (!str)
		{
			*exit_code = 2;
			return (0);
		}

		if (is_integer(str))
		{
			*exit_code = ft_atoi(str);
			*exit_code %= 256;
			if (*exit_code < 0)
=======
		return (1); // Indicate error (don't exit)
	}
	if (argv[1])
	{
		if (is_integer(argv[1]))
		{
			*exit_code = ft_atoi(argv[1]);
			// bash calculates exit code modulo 256
			*exit_code %= 256;
			if (*exit_code < 0) // Ensure positive result
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
				*exit_code += 256;
		}
		else
		{
			write_stderr("exit: numeric argument required\n");
			*exit_code = 2;
		}
<<<<<<< HEAD
		free(str);
	}
	return (0);
}


=======
	}
	return (0); // Indicate success (should exit)
}

>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
/// the last, 2 leaki
int bi_exit(char *argv[], t_resources *res)
{
    int exit_code;
    int should_exit;

    exit_code = g_exit_status;
    ft_putstr_fd("exit\n", STDOUT_FILENO);
    should_exit = parse_exit_args(argv, &exit_code);
    if (should_exit == 1)
		return (1);
<<<<<<< HEAD
    cleanup(res); 
	rl_clear_history(); 
=======
      
    // Clean up all resources
    cleanup(res); // This handles res->input, tokens, cmds, env
	rl_clear_history(); // Clear readline history
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
    exit(exit_code);
}
