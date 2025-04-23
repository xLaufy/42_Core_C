#include "inc/minishell.h"

int     g_exitcode;
void	set_exitcode(int32_t status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			g_exitcode = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		g_exitcode = WEXITSTATUS(status);
}



void handle_sigint(int sig)
{
    (void)sig;
        write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint); // Ctrl-C
	signal(SIGQUIT, SIG_IGN); // Ctrl-\ nic nie robi
}
