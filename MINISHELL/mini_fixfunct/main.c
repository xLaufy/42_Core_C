#include "inc/minishell.h"
#include <mcheck.h>

int main(int argc, char **argv, char **envp)
{
    t_env *env;
    char *input;
    t_resources res;

    (void)argc;
    (void)argv;
    init_shell(envp, &env);
    init_resources(&res);
    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
        {
            res.env = env;
            cleanup(&res);
            free(input);
            handle_eof(g_exit_status, &res);
        }
        if (*input)
        {
            add_history(input);
            process_input(input, &env);
        }
        else
            free(input);
    }
    return (0);
}
