#include "../inc/minishell.h"

void	write_stderr(char *str)
{
	write(2, str, ft_strlen(str));
}

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
			return (0);
		i++;
	}
	return (1);
}


int parse_exit_args(char *argv[], int *exit_code)
{
	char *str = NULL;

	if (argv[1] && argv[2])
	{
		write_stderr("exit: too many arguments\n");
		*exit_code = 1;
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
				*exit_code += 256;
		}
		else
		{
			write_stderr("exit: numeric argument required\n");
			*exit_code = 2;
		}
		free(str);
	}
	return (0);
}


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
    cleanup(res); 
	rl_clear_history(); 
    exit(exit_code);
}
