#include "inc/minishell.h"

int	is_builtin(char *cmd)
{
	if ((!strcmp(cmd, "echo") || !strcmp(cmd, "cd") || !strcmp(cmd, "pwd")
			|| !strcmp(cmd, "export") || !strcmp(cmd, "unset") || !strcmp(cmd,
				"env") || !strcmp(cmd, "exit")))
		return (1);
	else
		return (0);
}

int run_builtin(t_cmd *cmd, t_resources *res)
{
    if (!strcmp(cmd->args[0], "echo"))
        return (bi_echo(cmd->args));
    if (!strcmp(cmd->args[0], "pwd"))
        return (bi_pwd(cmd->args));
    if (!strcmp(cmd->args[0], "exit"))
        return (bi_exit(cmd->args, res));
    if (!strcmp(cmd->args[0], "unset"))
        return (bi_unset(cmd->args, &res->env));
    if (!strcmp(cmd->args[0], "export"))
        return (bi_export(cmd->args, &res->env));
    if (!strcmp(cmd->args[0], "env"))
        return (bi_env(cmd->args, &res->env));
    if (!strcmp(cmd->args[0], "cd"))
        return (bi_cd(cmd->args, &res->env));
    return (0);
}

int handle_parent_builtin(t_cmd *cmd, t_fds fds, t_resources *res)
{
    if (setup_redirections(cmd->redirections) < 0)
    {
        restore_std_fds(fds.stdin, fds.stdout);
        g_exit_status = 1;
        return (1);
    }
    
    g_exit_status = run_builtin(cmd, res);
    
    restore_std_fds(fds.stdin, fds.stdout);
    return (1);
}


int handle_single_builtin(t_cmd *cmds, t_resources *res)
{
    if (is_builtin(cmds->args[0]) && !cmds->next)
    {
        // Create a t_fds structure with duplicated file descriptors
        t_fds fds;
        fds.stdin = dup(STDIN_FILENO);
        fds.stdout = dup(STDOUT_FILENO);
        
        // Pass the t_fds structure to handle_parent_builtin
        handle_parent_builtin(cmds, fds, res);
        
        return (1);
    }
    return (0);
}#include "inc/minishell.h"

void free_env_node(t_env *node)
{
    if (node)
    {
        free(node->key);
        free(node->value);
        free(node);
    }
}

t_env *env_new(const char *key, const char *value)
{
	t_env *new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);

	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->is_exported = 1;
	new->next = NULL;

	if (!new->key || !new->value)
	{
		free(new->key);
		free(new->value);
		free(new);       // ✅ Prevent orphaning the struct
		return (NULL);
	}
	return (new);
}


t_env	*get_env(const char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}


char	*get_env_value(const char *key, t_env *env)
{
	t_env	*var;

	var = get_env(key, env);
	if (!var)
		return (NULL);
	return (var->value);
}



// dodaje new node to the end of a linked list of t_env nodes
void	env_add_back(t_env **env, t_env *new_node)
{
	t_env	*last;
	// if empty list, zrob nowy node
	if (!*env)
	{
		*env = new_node;
	}
	else
	{
		last = *env;
		// dopoki ten node ma nastepny node,
		// keep going(last idzie a do final node)
		while (last->next)
		{
			last = last->next;
		}
		last->next = new_node;
	}
}
#include "inc/minishell.h"

void	execute(t_cmd *cmds, t_env **env)
{
	int	stdin_copy;
	int	stdout_copy;
	int	prev_pipe_read;

	t_resources res = {0}; // Initialize resources structure
	// Populate resources for proper cleanup in case of exit
	init_resources(&res);
	res.env = *env;
	res.cmds = cmds;
	// Note: tokens and input would need to be provided from the caller
	// For now, set them to NULL
	if (!init_execution(cmds, &stdin_copy, &stdout_copy))
	{
		free_cmds(cmds);
		free_env(*env);
		return ;
	}
	// Pass the resources struct to handle_single_builtin
	if (handle_single_builtin(cmds, &res))
		return ;
	prepare_pipeline_execution(cmds, &prev_pipe_read);
	execute_pipeline(cmds, env, &prev_pipe_read);
	cleanup_execution(stdin_copy, stdout_copy);
}

int	setup_pipe_and_fork(t_cmd *cmd, int *pipe_fd)
{
	pid_t	pid;

	if (cmd->next && pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (0);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
		return (0);
	}
	return (pid);
}

void	handle_child_process(t_cmd *cmd, t_env **env, int prev_pipe_read,
		int *pipe_fd)
{
	t_resources	res;

	res.env = *env;
	res.cmds = cmd;
	res.tokens = NULL; // Tokens are not used here
	res.input = NULL;  // Input is not used here
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_pipe_read != -1)
	{
		dup2(prev_pipe_read, STDIN_FILENO);
		close(prev_pipe_read);
	}
	if (cmd->next)
		dup2(pipe_fd[1], STDOUT_FILENO);
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	if (setup_redirections(cmd->redirections) < 0)
	{
		free_cmds(cmd);
		free_env(*env);
		exit(1);
	}
	if (is_builtin(cmd->args[0]))
		exit(run_builtin(cmd, &res)); // Pass res instead of env
	else
		execute_external(cmd, *env);
}

void	execute_external(t_cmd *cmd, t_env *env)
{
	char *path = NULL;
	char **env_arr = NULL;

	path = get_exec_path(cmd->args[0], env);
	env_arr = env_to_arr(env);

	if (!path)
	{
		// get_exec_path couldn't find it (either not in PATH or explicit path didn't exist)
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		// Check if original command contained '/' to mimic bash message difference
		if (ft_strchr(cmd->args[0], '/'))
		{
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		}
		else
		{
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
		}
		ft_free_split(env_arr);
		// Don't free path, it's NULL
		exit(127);
	}

	// We have a path, attempt execution
	execve(path, cmd->args, env_arr);

	// ---- execve failed if we reach here ----
	int saved_errno = errno;
	struct stat statbuf;

	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		// Report error using the original command name
	ft_putstr_fd(": ", STDERR_FILENO);

	if (saved_errno == ENOENT)
	{
		// This case might be less likely now if get_exec_path uses stat,
		// but keep for robustness / edge cases.
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		ft_free_split(env_arr);
		free(path);
		exit(127);
	}
	else if (saved_errno == EACCES)
	{
		// Check if it's a directory first
		if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		{
			ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		}
		else
		{
			ft_putstr_fd("Permission denied\n", STDERR_FILENO);
		}
		ft_free_split(env_arr);
		free(path);
		exit(126);
	}
	// Add other specific errno checks if needed (e.g.,
	//	ENOEXEC for script format error)
	else
	{
		// Generic fallback
		ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		ft_free_split(env_arr);
		free(path);
		exit(126); // 126 for "command invoked cannot execute" type errors
	}
}#include "inc/minishell.h"


void cleanup(t_resources *res)
{
	if (res->input)
	{
		free(res->input);
	}
	if (res->tokens)
	{
		free_tokens(res->tokens);
	}
	if (res->cmds)
	{
		free_cmds(res->cmds);
	}
	if (res->env)
	{
		free_env(res->env);
	}
	rl_clear_history();
}

// //moje
void	process_command(t_cmd *cmd, t_env **env, int prev_pipe_read,
		int *pipe_fd)
{
	pid_t pid;

	pid = setup_pipe_and_fork(cmd, pipe_fd);
	if (pid == 0)
		handle_child_process(cmd, env, prev_pipe_read, pipe_fd);
}

int	init_execution(t_cmd *cmds, int *stdin_copy, int *stdout_copy)
{
	*stdin_copy = dup(STDIN_FILENO);
	*stdout_copy = dup(STDOUT_FILENO);
	if (!cmds || !cmds->args || !cmds->args[0])
	{
		ft_putstr_fd("minishell: null command\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

void	cleanup_execution(int stdin_copy, int stdout_copy)
{
	setup_signals_for_prompt();
	restore_std_fds(stdin_copy, stdout_copy);
}
// puste ify - usunac czy przydadza sie na
// usuniecie malo czytelnych w innych funkcjach?
///dziala dla testera wszystko, ponizej po naprawieniu heredoca,
// //kiedy spieprzyl sie przypadek 70
// void	wait_for_children(pid_t last_cmd_pid)
// {
// 	int status;
// 	pid_t pid;
// 	int last_cmd_status = 0; // Store the status of the specific last command
// 	int found_last = 0;
// 	// Flag to track if we found the last command's status

// 	// If last_cmd_pid is 0 or -1 (e.g., fork failed),
// 	//	default status might be appropriate
// 	if (last_cmd_pid <= 0)
// 	{
// 		// Decide on default behavior, maybe keep g_exit_status as is,
// 		//	or set to error?
// 		// Setting to 1 if last_cmd_pid indicates failure might be reasonable.
// 		// For now,
// 		//	we'll let it default based on what was last set if no children are waited for.
// 		// Or, if fork failed, an error might have already been set.
// 	}

// 	while ((pid = waitpid(-1, &status, 0)) > 0) // Wait for ANY child
// 	{
// 		if (pid == last_cmd_pid)
// 		// Is this the specific last command we care about?
// 		{
// 			found_last = 1; // Mark that we found it
// 			if (WIFEXITED(status))
// 			{
// 				last_cmd_status = WEXITSTATUS(status); // Get its exit status
// 			}
// 			else if (WIFSIGNALED(status))
// 			{
// 				int signal_num = WTERMSIG(status);
// 				last_cmd_status = 128 + signal_num;
// 				// Calculate signal exit status

// 				// Optional: Print signal messages like bash
// 				if (signal_num == SIGQUIT)
// 				{
// 					// Bash prints "Quit (core dumped)" - adjust as needed
// 					ft_putstr_fd("Quit\n", STDERR_FILENO);
// 				}
// 				else if (signal_num == SIGSEGV)
// 				{
// 					ft_putstr_fd("Segmentation fault\n", STDERR_FILENO);
// 				}
// 				// SIGINT is handled by the signal handler setting g_exit_status,
// 				// but waitpid will still report it here. last_cmd_status will be 130.
// 			}
// 			// No need to set g_exit_status here inside the loop
// 		}
// 		// We continue waiting even after finding the last command to ensure
// 		// all children in the pipeline are reaped and no zombies are left.
// 	}

// 	// After waiting for all children, set the global exit status
// 	// ONLY if we actually successfully waited for the last command.
// 	// If last_cmd_pid was valid (>0) but we never found it (waitpid error?),
// 	// maybe indicate an error. For now, we only set if found.
// 	if (last_cmd_pid > 0 && found_last)
// 	{
// 		g_exit_status = last_cmd_status;
// 	}
// 	else if (last_cmd_pid <= 0)
// 	{
// 		// Handle cases where there was no pipeline / fork failed for last cmd.
// 		// Perhaps g_exit_status was already set by a builtin or init_execution failure.
// 		// If no children were waited for at all (waitpid returned
// 		//	-1 immediately with ECHILD),
// 		// g_exit_status should reflect whatever happened before the wait loop.
// 		// So, maybe do nothing here if last_cmd_pid was invalid/not found.
// 	}

// 	// Error handling for waitpid itself
// 	if (pid == -1 && errno != ECHILD)
// 	{
// 		perror("minishell: waitpid");
// 		// Optionally set g_exit_status to indicate a wait error
// 		// g_exit_status = 1;
// 	}
// }




void	wait_for_children(pid_t last_cmd_pid)
{
	int status;
	pid_t pid;
	int last_cmd_status = 0; // Store the status of the specific last command
	int found_last = 0;      // Flag to track if we found the last command's status
	int printed_pipe_msg = 0; // Flag to potentially avoid printing "Broken pipe" multiple times

	// Initial status if last_cmd_pid is invalid (e.g., fork failed before loop)
	// If fork failed for the last command, execution might have already set
	// g_exit_status to an error code (like 1). We should preserve that if
	// no children are successfully waited for.
	// If last_cmd_pid <= 0, we can't rely on found_last/last_cmd_status later.

	while ((pid = waitpid(-1, &status, 0)) > 0) // Wait for ANY child
	{
		// --- Check status of the specific last command ---
		if (pid == last_cmd_pid)
		{
			found_last = 1; // Mark that we found it
			if (WIFEXITED(status))
			{
				last_cmd_status = WEXITSTATUS(status); // Get its normal exit status
			}
			else if (WIFSIGNALED(status)) // If the last command itself was signaled
			{
				int signal_num = WTERMSIG(status);
				last_cmd_status = 128 + signal_num; // Calculate its signal exit status
                // Don't print signal messages here yet, handle below for consistency
			}
            // else: Handle other termination types if necessary (stopped, continued)
		}

		// --- Check for signaled termination for *any* child waited for in this loop ---
		// This is where we print messages like "Broken pipe", "Quit", etc.
		if (WIFSIGNALED(status))
		{
			int signal_num = WTERMSIG(status);

            // Print signal messages like bash - DO THIS FOR *ANY* CHILD THAT SIGNALED
            // We print these messages even if the process wasn't the 'last_cmd_pid'
			if (signal_num == SIGQUIT)
			{
				// Bash prints "Quit (core dumped)" - adjust as needed
				// Check WCOREDUMP(status) if you want to add "(core dumped)"
				ft_putstr_fd("Quit", STDERR_FILENO);
                if (WCOREDUMP(status))
                    ft_putstr_fd(" (core dumped)", STDERR_FILENO);
                ft_putstr_fd("\n", STDERR_FILENO);
			}
			else if (signal_num == SIGSEGV)
			{
				ft_putstr_fd("Segmentation fault", STDERR_FILENO);
                if (WCOREDUMP(status))
                    ft_putstr_fd(" (core dumped)", STDERR_FILENO);
                ft_putstr_fd("\n", STDERR_FILENO);
			}
			else if (signal_num == SIGPIPE) // SIGPIPE is usually signal 13
			{
                // Optional: Only print "Broken pipe" once per pipeline wait
                if (!printed_pipe_msg)
                {
				    ft_putstr_fd("Broken pipe\n", STDERR_FILENO);
                    printed_pipe_msg = 1; // Set flag after printing
                }
                // Note: If multiple processes get SIGPIPE concurrently, Bash might
                // technically print it multiple times, but often only one matters.
                // Printing once is usually sufficient for mimicking the common case.
			}
            // else if (signal_num == SIGINT) {
            //     // We usually don't print anything extra for SIGINT here,
            //     // as the newline is typically handled by the signal handler.
            //     // The exit status (130) will be set correctly if it was the last command.
            // }
			// Add checks for other signals if needed
		}
		// We continue waiting even after finding the last command to ensure
		// all children in the pipeline are reaped and no zombies are left.
	} // End of while loop

	// --- Set the final global exit status ---
	// After waiting for all children, set the global exit status based
	// on the termination status of the *last* command in the pipeline.
	if (last_cmd_pid > 0 && found_last)
	{
		// We successfully found and waited for the last command.
		// last_cmd_status contains either its exit code or signal status (128+sig).
		g_exit_status = last_cmd_status;
	}
	else if (last_cmd_pid <= 0)
	{
		// Handle cases where there was no valid last command pid
		// (e.g., single command, fork failure before loop).
		// In this scenario, g_exit_status should likely retain the value
		// it had before wait_for_children was called (e.g., error status
		// from setup, or status from a single builtin).
		// If waitpid returned ECHILD immediately (no children), we also fall here.
		// So, we explicitly *don't* modify g_exit_status here.
	}
    else // (last_cmd_pid > 0 && !found_last)
    {
        // This case is strange: We had a valid last PID, but waitpid finished
        // without finding it (perhaps waitpid failed with EINTR or another error).
        // It might be safer to not modify g_exit_status or set it to a generic error.
        // Let's leave g_exit_status as is for now.
    }


	// --- Error handling for waitpid itself ---
	if (pid == -1 && errno != ECHILD)
	{
		// waitpid failed for a reason other than "no child processes"
		perror("minishell: waitpid");
		// Optionally set g_exit_status to indicate a wait error,
		// although the last command's status might be more relevant.
		// Overwriting might obscure the pipeline result.
		// Consider if setting g_exit_status = 1 here is appropriate.
	}
}#include "inc/minishell.h"

int		g_exit_status = 0; // Globalna zmienna stanu

// Count the number of nodes in the environment list
int	count_env_nodes(t_env *env)
{
	int		count;
	t_env	*current;
	// var: env(param), count, current = 3 <= 5 OK

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

//Create a single "KEY=VALUE" string entry.
//Returns the allocated string or NULL on error (malloc fail, NULL key/value).
char	*create_env_entry(t_env *node)
{
	char	*key_eq;
	char	*entry;
	// var: node(param), key_eq, entry = 3 <= 5 OK
	// Check for NULL key or value within the node first
	if (!node || !node->key || !node->value)
		return (NULL);
	key_eq = ft_strjoin(node->key, "=");
	if (!key_eq)
		return (NULL);
	entry = ft_strjoin(key_eq, node->value);
	free(key_eq); // Free intermediate string immediately
	if (!entry)
		return (NULL); // Check final string allocation
	return (entry);
}

//Free the partially allocated array on error
//Frees arr[0] up to arr[count-1], then frees arr itself
void	free_array_on_error(char **arr, int count)
{
	int	i; // var: arr(param), count(param), i = 3 <= 5 OK
	i = 0;
	while (i < count)
	{
		free(arr[i]); // Free only the elements that were successfully allocated
		i++;
	}
	free(arr); // Free the array container
}

int	populate_env_array(t_env *env, char **arr)
{
	int		i;
	t_env	*current;
	char	*entry;

	i = 0;
	current = env;
	while (current)
	{
		entry = create_env_entry(current);
		if (!entry)
			return (-1); // Signal error during entry creation
		arr[i] = entry;
		i++;
		current = current->next;
	}
	arr[i] = NULL; // Null-terminate the array
	return (i); // Return the count of successfully added entries
}

/*
 * Main Function: Converts the environment linked list to a NULL-terminated
 * array of strings ("KEY=VALUE"). Returns NULL on any error.
 */
char	**env_to_arr(t_env *env)
{
	int		count;
	char	**arr;
	int		populate_result;

	if (!env)
		return (NULL);
	count = count_env_nodes(env);
	arr = malloc((count + 1) * sizeof(char *));
	if (!arr)
	{
		perror("minishell: malloc");
		return (NULL);
	}
	populate_result = populate_env_array(env, arr);
	if (populate_result == -1) // Check for failure
	{
		free_array_on_error(arr, count); // Free all allocated entries
		return (NULL);
	}
	return (arr);
}


void init_resources(t_resources *res)
{
	res->input = NULL;
	res->tokens = NULL;
	res->cmds = NULL;
	res->env = NULL;
}#include "inc/minishell.h"


/**
 * Wyodrębnia nazwę zmiennej środowiskowej (np. z "$USER" → "USER").
 * - str: wskaźnik do znaku '$' (np. "USER=john").
 * Zwraca: nazwa zmiennej (malloc).
 */
char	*extract_var_name(char *str)
{
	int len;

	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
	{
		len++;
	}
	return (ft_substr(str, 0, len));
}

void	expand_variables(t_token *tokens, t_env *env)
{
	char *expanded;

	while (tokens)
	{
		if (tokens->type == T_SINGLE_QUOTED)
		{
			// NIE ROBIMY NIC
		}
		else if (tokens->type == T_DOUBLE_QUOTED || tokens->type == T_WORD)
		{
			expanded = expand_str(tokens->value, env);
			if (expanded)
			{
				free(tokens->value);
				tokens->value = expanded;
			}
		}
		tokens = tokens->next;
	}
}

int	handle_braced_var(char **result, char *str, int i, t_env *env)
{
	int start;
	char *var_name;
	char *var_value;

	start = i + 2;
	while (str[i] && str[i] != '}')
		i++;
	if (str[i] == '}')
	{
		var_name = ft_substr(str, start, i - start);
		var_value = get_env_value(var_name, env);
		*result = ft_strjoin_free(*result, var_value ? var_value : "");
		free(var_name);
		i++;
	}
	return (i);
}
int	handle_standard_var(char **result, char *str, int i, t_env *env)
{
	char *var_name;
	char *var_value;

	var_name = extract_var_name(str + i + 1);
	if (var_name[0] == '\0')
	{
		*result = ft_strjoin_free(*result, "$");
		free(var_name);
		return (i + 1);
	}
	var_value = get_env_value(var_name, env);
	*result = ft_strjoin_free(*result, var_value ? var_value : "");
	i += ft_strlen(var_name) + 1;
	free(var_name);
	return (i);
}

char	*expand_str(char *str, t_env *env)
{
	char *result;
	int i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (is_special_var(str, i))
			i = handle_special_var(&result, i);
		else if (is_braced_var(str, i))
			i = handle_braced_var(&result, str, i, env);
		else if (is_standard_var(str, i))
			i = handle_standard_var(&result, str, i, env);
		else
		{
			char *old_result = result;
			result = ft_strjoin_char(old_result, str[i]);
			if (!result)
			{
				free(old_result);
				return (NULL);
			}
			i++;
		}
	}
	return (result);
}

////from expandet utils.c

int	is_special_var(char *str, int i)
{
	return (str[i] == '$' && str[i + 1] == '?');
}

int	is_braced_var(char *str, int i)
{
	return (str[i] == '$' && str[i + 1] == '{');
}

int	is_standard_var(char *str, int i)
{
	return (str[i] == '$');
}

void	append_char(char **result, char c)
{
	char tmp[2];
	char *new_result;

	tmp[0] = c;
	tmp[1] = '\0';
	new_result = ft_strjoin(*result, tmp); // Don't free here
	if (!new_result)
	{
		free(*result);
		*result = NULL;
		return ;
	}
	*result = new_result;
}

int	handle_special_var(char **result, int i)
{
	char *exit_status_str = ft_itoa(g_exit_status);
	if (!exit_status_str)
		return (-1); // Error handling

	char *new_result = ft_strjoin_free(*result, exit_status_str);
	free(exit_status_str); // Free the string after using it

	if (!new_result)
		return (-1);

	*result = new_result;
	return (i + 2);
}
#include "inc/minishell.h"

//caly file - podzielony init_env
char	*extract_key(char *env_var, int *key_len)
{
	char	*eq;
	char	*key;

	eq = ft_strchr(env_var, '=');
	if (!eq)
		return (NULL);
	*key_len = eq - env_var;
	key = ft_substr(env_var, 0, *key_len);
	return (key);
}

int	handle_env_node_creation(char *key, char *value, t_env **env)
{
	t_env	*new_node;

	new_node = env_new(key, value);
	if (!new_node)
	{
		free(key);
		free(value);
		perror("minishell: malloc");
		free_env(*env);
		exit(EXIT_FAILURE);
	}
	env_add_back(env, new_node);
	free(key);
	free(value);
	return (1);
}

int	process_single_env_var(char *env_var, t_env **env)
{
	char	*key;
	char	*value;
	int		key_len;

	key = extract_key(env_var, &key_len);
	if (!key)
		return (0);
	value = ft_strdup(ft_strchr(env_var, '=') + 1);
	if (!value)
	{
		free(key);
		return (0);
	}
	return (handle_env_node_creation(key, value, env));
}

void	init_env(char **envp, t_env **env)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		process_single_env_var(envp[i], env);
		i++;
	}
}
#include "inc/minishell.h"


// dodaje tokeny for a single command line, to the beginning of the linked list
void	add_token(t_token **tokens, char *value, t_token_type type)
{
	t_token *new;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = ft_strdup(value); // MUST duplicate the string
	if (!new->value)
	{
		free(new);
		return ;
	}
	new->type = type;
	new->next = *tokens;
	*tokens = new;
}

t_token	*reverse_tokens(t_token *tokens)
{
	t_token *prev;
	t_token *current;
	t_token *next;

	prev = NULL;
	current = tokens;
	next = NULL;
	while (current)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	return (prev);
}

void	handle_input_char(char *input, int *i, char **current_word,
		t_token **tokens)
{
	static int error = 0;

	if (ft_isspace(input[*i]))
		skip_space(i, current_word, tokens);
	else if (is_quote(input[*i]))
	{
		// Zakończ słowo przed obsługą cytatu
		if (*current_word)
			finalize_current_word(tokens, current_word, 0);
				// 0 dla standardowego słowa (brak cytatu)
		handle_quoted(input, i, tokens, &error);           
			// Obsługujemy cudzysłowy
	}
	else if (is_pipe(input[*i]))
		handle_pipe(i, current_word, tokens);
	else if (is_redirect(input[*i]))
		handle_redirect_token(input, i, current_word, tokens);
	else
		append_char_to_word(current_word, input[(*i)++]);
}
#include "inc/minishell.h"

int	is_pipe(char c)
{
	return (c == '|');
}

int	is_redirect(char c)
{
	return (c == '>' || c == '<');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

t_quote	get_quote_type(char c)
{
	if (c == '\'')
		return (SINGLE_QUOTE);
	return (DOUBLE_QUOTE);
}

t_token_type	get_token_type_from_quote(t_quote quote)
{
	if (quote == SINGLE_QUOTE)
		return (T_SINGLE_QUOTED);
	return (T_DOUBLE_QUOTED);
}#include "inc/minishell.h"


int	handle_quote(char *input, int *i, t_token **tokens, int *error)
{
	t_quote current_quote;
	char *quoted;
	t_token_type quote_type;

	current_quote = get_quote_type(input[*i]);
	(*i)++;
	quoted = read_quoted_content(input, i, current_quote, error);
	if (*error)
		return (ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
				STDERR_FILENO), 1);
	quote_type = get_token_type_from_quote(current_quote);
	add_token(tokens, quoted, quote_type);
	free(quoted);
	return (0);
}

char	*read_quoted_content(char *input, int *i, t_quote quote, int *error)
{
	char quote_char = (quote == SINGLE_QUOTE) ? '\'' : '"';
	int start = *i;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;

	if (input[*i] != quote_char)
	{
		*error = 1; // Ustaw błąd jeśli nie znaleziono zamykającego cytatu
		return (NULL);
	}

	char *content = ft_substr(input, start, *i - start);
	(*i)++; // Zwiększ indeks, aby przejść po zamykającym cudzysłowie
	return (content);
}

void	free_tokens(t_token *tokens)
{
	t_token *tmp;

	while (tokens)
	{
		tmp = tokens;
		if (tokens->value)
			free(tokens->value); 
		tokens = tokens->next;
		free(tmp);
	}
}
#include "inc/minishell.h"


void	skip_space(int *i, char **word, t_token **tokens)
{
	if (*word)
		finalize_current_word(tokens, word, 0);
	(*i)++;
}

void	handle_pipe(int *i, char **word, t_token **tokens)
{
	if (*word)
		finalize_current_word(tokens, word, 0);
	add_token(tokens, "|", T_PIPE);
	(*i)++;
}

int	handle_quoted(char *input, int *i, t_token **tokens, int *error)
{
	t_quote current_quote;
	char *quoted;

	current_quote = get_quote_type(input[*i]);
	(*i)++; // Pomijamy otwierający cudzysłów

	quoted = read_quoted_content(input, i, current_quote, error);
	if (*error)
	{
		ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
			STDERR_FILENO);
		return (1);
	}

	// Dodajemy token z odpowiednim typem w zależności od typu cytatu
	t_token_type quote_type = get_token_type_from_quote(current_quote);
	add_token(tokens, quoted, quote_type);

	free(quoted); // Zwolnij pamięć
	return (0);
}

t_token	*handle_unexpected_quote(char *word, t_token *tokens)
{
	free(word);
	free_tokens(tokens);
	return (NULL);
}
#include "inc/minishell.h"


void	handle_redirect_token(char *input, int *i, char **word,
		t_token **tokens)
{
	if (*word)
		finalize_current_word(tokens, word, 0);
	handle_redirection(input, i, tokens);
}

void	validate_redirection_syntax(t_token **tokens)
{
	if (*tokens && (*tokens)->next)
	{
		if ((*tokens)->next->type != T_WORD
			&& (*tokens)->next->type != T_SINGLE_QUOTED
			&& (*tokens)->next->type != T_DOUBLE_QUOTED)
		{
			ft_putstr_fd("minishell: syntax error:\n", STDERR_FILENO);
			ft_putstr_fd("Missing file name after redirection\n",
				STDERR_FILENO);
			*tokens = NULL;
			// To indicate an error and avoid further processing
		}
	}
}
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
#include "inc/minishell.h"

int	handle_quoted_token(char *input, int *i, t_token **tokens, int *error)
{
	t_quote			current_quote;
	char			*quoted_content;
	t_token_type	quote_type;

	current_quote = get_quote_type(input[*i]);
	(*i)++; // Move past the opening quote
	// read_quoted_content reads *only* the inner content
	quoted_content = read_quoted_content(input, i, current_quote, error);
	if (*error || quoted_content == NULL) // Check error flag OR malloc failure
	{
		// read_quoted_content might set error but return NULL without msg
		if (*error && quoted_content == NULL)
		{
			ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
				STDERR_FILENO);
		}
		else if (quoted_content == NULL)
		{
			// Malloc error likely
			perror("minishell: malloc error in read_quoted_content");
			*error = 1; // Ensure error is set
		}
		// Do not free quoted_content if it's NULL
		// If *error is set but quoted_content is not NULL (shouldn't happen?),
		//	free it.
		if (quoted_content)
			free(quoted_content);
		return (1); // Indicate failure
	}
	// Determine token type based on the quote character used
	quote_type = get_token_type_from_quote(current_quote);
	// Add the token with the correct type and the *inner* content
	add_token(tokens, quoted_content, quote_type);
	free(quoted_content); // Free the temporary content string
	// i is already advanced past the closing quote by read_quoted_content
	return (0); // Indicate success
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next; // Save next pointer before freeing
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	init_shell(char **envp, t_env **env)
{
	setup_signals_for_prompt();
	*env = NULL;
	init_env(envp, env);
}

void	handle_eof(int exit_status, t_resources *res)
{
	cleanup(res);
	// Don't free env here, it's already freed in cleanup
	// free_env(env);  <- Remove this line
	// Clear readline history
	rl_clear_history();
	// Print a newline for better UX when exiting with Ctrl+D
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(exit_status);
}


// //moje
// void	process_input(char *input, t_env **env)
// {
// 	t_token	*tokens;
// 	t_cmd	*cmds;

// 	tokens = NULL;
// 	cmds = NULL;
// 	t_fds fds;             // For saving/restoring std fds for parent builtins
// 	t_resources res = {0}; // Resources for cleanup (esp. for bi_exit)
// 	// --- Setup Resources ---
// 	res.input = input; // Track original input pointer (though we free it here)
// 	res.env = *env;    // Track environment pointer
// 	// --- Tokenize ---
// 	tokens = tokenize_input(input);
// 	if (!tokens)
// 	{
// 		free(input); // Free original input if tokenization fails
// 		// g_exit_status might be set by tokenizer on syntax error
// 		return ;
// 	}
// 	res.tokens = tokens; // Track tokens
// 	// --- Expand Variables ---
// 	expand_variables(tokens, *env);
// 	// --- Parse Tokens into Commands ---
// 	cmds = parse_tokens(tokens);
// 	if (!cmds)
// 	{
// 		// Parsing failed (syntax error should set g_exit_status)
// 		free_tokens(tokens);
// 		free(input);
// 		return ;
// 	}
// 	res.cmds = cmds; // Track commands
// 	// --- Handle Argument Shifting for Empty Command Names (like $EMPTY cmd) ---
// 	shift_empty_args(cmds); // Modify cmds->args in place if needed
// 	// --- Handle Commands that are NOW Empty (like $EMPTY alone,
// 	//	or after shift) ---
// 	// Check if the *first* command is now effectively empty (no args[0] or args[0] is "")
// 	// Note: shift_empty_args ensures args[0] isn't "" if args[1] existed.
// 	// This check catches "$EMPTY" alone or syntax resulting in no args.
// 	if (!cmds->args || !cmds->args[0] || cmds->args[0][0] == '\0')
// 	{
// 		// Check specifically for the "$EMPTY" alone case (single command,
// 		//	now empty)
// 		if (!cmds->next && (!cmds->args || !cmds->args[0]
// 				|| cmds->args[0][0] == '\0'))
// 		{
// 			g_exit_status = 0; // Bash exits 0 for just "" or $EMPTY
// 		}
// 		// Otherwise, if parsing/shifting resulted in no command (e.g.,
// 		//	" > file "),
// 		// let the exit status remain or set to an error? Bash might set 0 or error.
// 		// For simplicity, we might just let it be 0 or the status from parsing.
// 		// Cleanup and return, skipping execution
// 		free_cmds(cmds);
// 		free_tokens(tokens);
// 		free(input);
// 		return ;
// 	}
// 	// --- End Empty Command Check ---
// 	// --- Proceed with Execution ---
// 	// At this point, cmds, cmds->args,
// 	//	and cmds->args[0] should be valid and non-empty.
// 	setup_signals_for_command(); // Set SIGINT/SIGQUIT handlers for execution
// 	// Check if it's a single command that is a builtin
// 	if (is_builtin(cmds->args[0]) && !cmds->next)
// 	{
// 		// Run single builtin in parent process
// 		fds.stdin = dup(STDIN_FILENO); // Backup std fds
// 		fds.stdout = dup(STDOUT_FILENO);
// 		// Pass full resources in case the builtin (like exit) needs to cleanup everything
// 		handle_parent_builtin(cmds, fds, &res);
// 		// handle_parent_builtin restores fds and sets g_exit_status
// 		// If bi_exit was called, we won't get here.
// 	}
// 	// Otherwise, it's an external command, a pipeline,
// 	// or a builtin in a pipeline
// 	else
// 	{
// 		// Execute external command(s) or pipeline (forks involved)
// 		execute(cmds, env); // execute() handles waiting and sets g_exit_status
// 	}
// 	setup_signals_for_prompt(); // Reset signal handlers for the next prompt
// 	// --- Cleanup for the current input ---
// 	// This cleanup runs unless bi_exit was called.
// 	free_cmds(cmds);
// 	free_tokens(tokens);
// 	free(input); // Free the original readline input string
// 					// Note: 'env' persists across inputs. 'res' is cleaned up if bi_exit called.
// }


void process_input(char *input, t_env **env)
{
    t_token     *tokens;
    t_cmd       *cmds;
    t_fds       fds;
    t_resources res = {0};

    // --- Setup Resources ---
    res.input = input;
    res.env = *env;

    // --- Tokenize ---
    tokens = tokenize_input(input);
    if (!tokens) { /* ... existing error handling ... */ return; }
    res.tokens = tokens;

    // --- Expand Variables ---
    expand_variables(tokens, *env);

    // --- Parse Tokens into Commands ---
    cmds = parse_tokens(tokens);
    if (!cmds) { /* ... existing error handling ... */ return; }
    res.cmds = cmds;

    // --- Handle Argument Shifting ---
    shift_empty_args(cmds);

    // --- Handle Empty Commands ---
    if (!cmds->args || !cmds->args[0] || cmds->args[0][0] == '\0') {
        /* ... existing empty command handling ... */
        // Make sure to free resources before returning
        free_cmds(cmds);
        free_tokens(tokens);
        free(input);
        return;
    }

    // --- >>> NEW: Handle Heredocs <<< ---
    if (handle_all_heredocs(cmds) != 0) {
        // Heredoc failed, error message/warning printed inside handle_heredoc
        // g_exit_status is set inside handle_all_heredocs or handle_heredoc
        close_heredoc_fds(cmds); // Close any FDs that might have been opened before failure
        free_cmds(cmds);
        free_tokens(tokens);
        free(input);
        return; // Skip execution
    }
    // --- >>> END NEW <<< ---

    // --- Proceed with Execution ---
    setup_signals_for_command();

    if (is_builtin(cmds->args[0]) && !cmds->next) {
        fds.stdin = dup(STDIN_FILENO);
        fds.stdout = dup(STDOUT_FILENO);
        // Pass full resources in case of exit
        handle_parent_builtin(cmds, fds, &res); // Note: exit() won't return
        // handle_parent_builtin now handles redirection setup internally
    } else {
        execute(cmds, env); // Handles waiting and sets g_exit_status
    }

    setup_signals_for_prompt(); // Reset signal handlers

    // --- >>> NEW: Close Heredoc FDs in Parent <<< ---
    close_heredoc_fds(cmds);
    // --- >>> END NEW <<< ---


    // --- Cleanup for the current input ---
    // Don't call cleanup(&res) here unless exit was called,
    // otherwise env will be freed.
    // cleanup is primarily for bi_exit.
    free_cmds(cmds);
    free_tokens(tokens);
    free(input);
}








void	shift_empty_args(t_cmd *cmds)
{
	t_cmd	*current_cmd;
	int		i;
	char	*empty_arg_to_free;

	current_cmd = cmds;
	while (current_cmd)
	{
		// Check if args exists, has at least two elements, and args[0] is empty
		if (current_cmd->args && current_cmd->args[0] && current_cmd->args[1]
			&& current_cmd->args[0][0] == '\0')
		{
			i = 0;
			empty_arg_to_free = current_cmd->args[0];
			// Save pointer to free
			// Shift pointers left
			while (current_cmd->args[i + 1])
			{
				current_cmd->args[i] = current_cmd->args[i + 1];
				i++;
			}
			current_cmd->args[i] = NULL; // Null-terminate the shifted array
			free(empty_arg_to_free);     // Free the original empty string arg
		}
		current_cmd = current_cmd->next;
	}
}

void	clean_cmds_tokens_input(t_cmd *cmds, t_token *tokens, char *input)
{
	if (cmds)
		free_cmds(cmds);
	if (tokens)
		free_tokens(tokens);
	if (input)
		free(input);
}

static int	ensure_current_word(char **current_word)
{
	if (*current_word == NULL)
	{
		*current_word = ft_strdup("");
		if (!*current_word)
		{
			perror("minishell: malloc error");
			return (0);
		}
	}
	return (1);
}

void	append_char_to_word(char **word, char c)
{
	char	*old_word;

	if (!ensure_current_word(word))
	{
		return ;
	}
	old_word = *word;
	*word = ft_strjoin_char(old_word, c);
	if (!*word)
	{
		perror("minishell: malloc error");
	}
}

int	handle_quoted_append(char *input, int *i, char **current_word, int *error,
		t_parse_state *state)
{
	t_quote	current_quote;
	char	quote_char;
	int		start;
	int		len;
	char	*content;
	char	*old_word;

	current_quote = get_quote_type(input[*i]);
	quote_char = (current_quote == SINGLE_QUOTE) ? '\'' : '"';
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote_char)
	{
		(*i)++;
	}
	if (input[*i] != quote_char)
	{
		ft_putstr_fd("Minishell: syntax error: unclosed quote\n",
			STDERR_FILENO);
		*error = 1;
		return (1);
	}
	// Ustawiamy flagę, jeśli napotkaliśmy pojedynczy cudzysłów
	if (current_quote == SINGLE_QUOTE)
	{
		state->single_quote_used = 1;
	}
	if (!ensure_current_word(current_word))
	{
		*error = 1;
		return (1);
	}
	len = *i - start;
	if (len > 0)
	{
		content = ft_substr(input, start, len);
		if (!content)
		{
			perror("minishell: malloc error");
			*error = 1;
			return (1);
		}
		old_word = *current_word;
		*current_word = ft_strjoin(old_word, content);
		free(old_word);
		free(content);
		if (!*current_word)
		{
			perror("minishell: malloc error");
			*error = 1;
			return (1);
		}
	}
	(*i)++;
	return (0);
}

void	finalize_current_word(t_token **tokens, char **current_word,
		int single_quote_used)
{
	t_token_type	type;

	if (*current_word != NULL)
	{
		type = single_quote_used ? T_SINGLE_QUOTED : T_WORD;
		add_token(tokens, *current_word, type);
		free(*current_word);
		*current_word = NULL;
	}
}

void	handle_redirection(char *input, int *i, t_token **tokens)
{
	char			first_char;
	t_token_type	type;
	char			op_str[3];

	first_char = input[*i];
	op_str[0] = first_char;
	(*i)++;
	if (input[*i] == first_char)
	{
		op_str[1] = first_char;
		op_str[2] = '\0';
		(*i)++;
		if (first_char == '>')
			type = T_APPEND;
		else
			type = T_HEREDOC;
		add_token(tokens, op_str, type);
	}
	else
	{
		op_str[1] = '\0';
		if (first_char == '>')
			type = T_REDIR_OUT;
		else
			type = T_REDIR_IN;
		add_token(tokens, op_str, type);
	}
}

void	parse_input_loop(char *input, t_parse_state *state)
{
	while (input[state->i] && !(state->error))
	{
		//|| input[state->i - 1] == ' '))
		if (input[state->i] == '#' && (state->i == 0
				|| ft_isspace(input[state->i - 1])))
			// Zatrzymaj przetwarzanie po napotkaniu znaku komentarza
			break ;
		if (ft_isspace(input[state->i]))
		{
			// Finalizuj jakiekolwiek słowo przed pominięciem spacji
			finalize_current_word(&state->tokens, &state->current_word,
				state->single_quote_used);
			state->i++; // Pomijamy spację
		}
		else if (is_pipe(input[state->i]))
		{
			// Finalizuj słowo, dodaj token dla pipe
			finalize_current_word(&state->tokens, &state->current_word,
				state->single_quote_used);
			add_token(&state->tokens, "|", T_PIPE);
			state->i++;
		}
		else if (is_redirect(input[state->i]))
		{
			// Finalizuj słowo przed obsługą operatora przekierowania
			finalize_current_word(&state->tokens, &state->current_word,
				state->single_quote_used);
			handle_redirection(input, &state->i, &state->tokens);
			// Dodaje token przekierowania
		}
		else if (is_quote(input[state->i]))
		{
			// Dodaj zawartość cytatu do aktualnie budowanego słowa
			if (handle_quoted_append(input, &state->i, &state->current_word,
					&state->error, state) != 0)
			{
				// Wystąpił błąd (niezamknięty cudzysłów lub błąd malloc)
				state->error = 1; // Ustawiamy flagę błędu
			}
		}
		else
		{
			// Dodaj zwykły znak do aktualnie budowanego słowa
			append_char_to_word(&state->current_word, input[state->i]);
			if (state->current_word == NULL && errno == ENOMEM)
			{ // Sprawdzamy, czy append_char się nie udał
				state->error = 1;
			}
			state->i++;
		}
	}
	// Po zakończeniu pętli, finalizuj jakiekolwiek pozostałe słowo
	if (!state->error)
	{ // Finalizujemy tylko, jeśli nie wystąpił błąd w trakcie pętli
		finalize_current_word(&state->tokens, &state->current_word,
			state->single_quote_used);
	}
}

t_token	*tokenize_input(char *input)
{
	t_parse_state	state;

	state.tokens = NULL;
	state.current_word = NULL;
	state.i = 0;
	state.error = 0;
	state.single_quote_used = 0; // Inicjalizujemy na 0
	parse_input_loop(input, &state);
	// Finalne słowo obsługiwane *wewnątrz* i *po* parse_input_loop
	if (state.error)
	{
		// Czyszczenie potencjalnie częściowo zbudowanego słowa i tokenów
		if (state.current_word)
			free(state.current_word);
		free_tokens(state.tokens);
		return (NULL);
	}
	return (reverse_tokens(state.tokens));
}
#include "inc/minishell.h"

/**
 * Obsługuje tokeny przekierowań w parserze.
 * - tokens: podwójny wskaźnik do aktualnego tokenu (np. `>` lub `<<`).
 * - cmd: bieżąca komenda, do której dodawane jest przekierowanie.
 */
// //moje
// void	handle_redirection_parser(t_token **tokens, t_cmd *cmd)
// {
// 	t_redir	*new_redir;

// 	new_redir = malloc(sizeof(t_redir));
// 	if (!new_redir)
// 	{
// 		perror("minishell");
// 		return ;
// 	}
// 	// Set redirection type based on token
// 	new_redir->type = (*tokens)->type;
// 	// Move to next token (should be filename)
// 	*tokens = (*tokens)->next;
// 	if (!*tokens || ((*tokens)->type != T_WORD
// 			&& (*tokens)->type != T_SINGLE_QUOTED
// 			&& (*tokens)->type != T_DOUBLE_QUOTED))
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token\n",
// 			STDERR_FILENO);
// 		free(new_redir);
// 		return ;
// 	}
// 	// Assign filename and add redirection to command
// 	new_redir->file = ft_strdup((*tokens)->value);
// 	new_redir->next = NULL;
// 	add_redir_to_cmd(cmd, new_redir);
// 	// Move to next token
// 	*tokens = (*tokens)->next;
// }

void handle_redirection_parser(t_token **tokens, t_cmd *cmd)
{
    t_redir *new_redir;

    new_redir = malloc(sizeof(t_redir));
    if (!new_redir)
    {
        perror("minishell: malloc");
        // Consider more robust error handling (e.g., freeing existing cmd structure)
        return; // Or exit, or set an error flag
    }
    new_redir->type = (*tokens)->type;
    new_redir->heredoc_fd = -1; // <<< Initialize here

    // Move to next token (should be filename/delimiter)
    *tokens = (*tokens)->next;
    if (!*tokens || ((*tokens)->type != T_WORD && (*tokens)->type != T_SINGLE_QUOTED &&
                      (*tokens)->type != T_DOUBLE_QUOTED))
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
        // Print the unexpected token if possible, or just a newline
         if (*tokens && (*tokens)->value) {
             ft_putstr_fd((*tokens)->value, STDERR_FILENO);
             ft_putstr_fd("'\n", STDERR_FILENO);
         } else {
            ft_putstr_fd("newline'\n", STDERR_FILENO);
         }
        g_exit_status = 2; // Syntax error
        free(new_redir);
        // Signal error back up the call chain if possible
        return;
    }
    // Assign filename/delimiter
    new_redir->file = ft_strdup((*tokens)->value);
     if (!new_redir->file) { // Handle strdup failure
        perror("minishell: malloc");
        free(new_redir);
        return; // Or signal error
    }
    new_redir->next = NULL;
    add_redir_to_cmd(cmd, new_redir);
    // Move to next token
    *tokens = (*tokens)->next;
}

/// do usuniecia, ponizej mam ta sama wersje
void free_cmd_contents(t_cmd *cmd)
{
	if (!cmd)
		return;

	if (cmd->args)
	{
		for (int i = 0; cmd->args[i]; i++)
			free(cmd->args[i]);
		free(cmd->args);
	}

	t_redir *redir = cmd->redirections;
	t_redir *tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = tmp;
	}
}


void	free_cmds(t_cmd *cmds)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmds;
	while (current != NULL)
	{
		next = current->next;
		free_cmd_contents(current);
		if (current->pipe_fd[0] != -1)
			close(current->pipe_fd[0]);
		if (current->pipe_fd[1] != -1)
			close(current->pipe_fd[1]);
		free(current);
		current = next;
	}
}

t_cmd	*process_token(t_token **tokens, t_cmd *current_cmd, t_cmd **cmds)
{
	char	*arg_copy;

	if (is_redirection((*tokens)->type))
	{
		handle_redirection_parser(tokens, current_cmd);
	}
	else if ((*tokens)->type == T_PIPE)
	{
		*tokens = (*tokens)->next; // Skip pipe token
	}
	else
	{
		arg_copy = ft_strdup((*tokens)->value);
		if (!arg_copy)
		{
			perror("minishell");
			free_cmds(*cmds);
			free_tokens(*tokens);
			return (NULL);
		}
		add_arg_to_cmd(current_cmd, arg_copy);
		*tokens = (*tokens)->next;
	}
	return (current_cmd);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*current_cmd;
	int		cmd_count;

	cmds = NULL;
	current_cmd = NULL;
	cmd_count = 0;
	while (tokens)
	{
		// Create new command if needed (start or after pipe)
		if (!current_cmd || tokens->type == T_PIPE)
		{
			cmd_add_back(&cmds, cmd_new());
			current_cmd = cmd_last(cmds);
			cmd_count++;
		}
		// Process current token
		current_cmd = process_token(&tokens, current_cmd, &cmds);
		if (!current_cmd) // Check for allocation failure
		{
			free_cmds(cmds);
			free_tokens(tokens);
			return (NULL);
		}
	}
	return (cmds);
}
#include "inc/minishell.h"

/**
 * Dodaje przekierowanie do listy w komendzie.
 */
void	add_redir_to_cmd(t_cmd *cmd, t_redir *new_redir)
{
	t_redir	*last;

	if (!cmd->redirections)
	{
		cmd->redirections = new_redir;
	}
	else
	{
		last = cmd->redirections;
		while (last->next)
		{
			last = last->next;
		}
		last->next = new_redir;
	}
}

t_cmd	*cmd_new(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirections = NULL;
	cmd->next = NULL;
	cmd->pipe_fd[0] = -1;
	cmd->pipe_fd[1] = -1;
	return (cmd);
}

t_cmd	*cmd_last(t_cmd *cmds)
{
	while (cmds && cmds->next)
	{
		cmds = cmds->next;
	}
	return (cmds);
}

// a pointer to a pointer allows the function to modify 
//the head of the list (first element)
// used to add a new command (new_cmd) t the end of the list of commands.
// appends a new t_cmd structure to the list,
//	it either reates the list if empty,
// or adds to the end if the list already has elemensrs
void	cmd_add_back(t_cmd **cmds, t_cmd *new_cmd)
{
	t_cmd	*last;

	if (!*cmds)
	{
		*cmds = new_cmd;
	}
	else
	{
		last = cmd_last(*cmds);
		last->next = new_cmd;
	}
}

int	is_redirection(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT || type == T_APPEND
		|| type == T_HEREDOC);
}
#include "inc/minishell.h"

void	add_arg_to_cmd(t_cmd *cmd, char *arg)
{
	int		count;
	char	**new_args;
	int		i;

	count = 0;
	i = 0;
	if (cmd->args)
		while (cmd->args[count])
			count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
	{
		perror("minishell");
		exit(EXIT_FAILURE); // Or handle more gracefully
	}
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = arg;
	new_args[count + 1] = NULL;
	if (cmd->args)
		free(cmd->args);
	cmd->args = new_args;
}
#include "inc/minishell.h"

void	prepare_pipeline_execution(t_cmd *cmds, int *prev_pipe_read)
{
	*prev_pipe_read = -1;
	setup_signals_for_command();
	(void)cmds;
}

void	execute_pipeline(t_cmd *cmds, t_env **env, int *prev_pipe_read)
{
	int		pipe_fd[2];
	pid_t	pid;

	pid_t last_pid = -1;       // Variable to store the PID of the last command
	t_cmd *current_cmd = cmds; // Use a temporary pointer to iterate
	while (current_cmd)
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		// Directly call setup_pipe_and_fork here to get the PID
		pid = setup_pipe_and_fork(current_cmd, pipe_fd);
		if (pid < 0)
		{
			// Fork failed, maybe handle error, break loop?
			// perror("minishell: fork failed in pipeline");
			// Consider setting an error status
			if (*prev_pipe_read != -1)
				close(*prev_pipe_read); // Cleanup pending read pipe
			break ;                      // Stop processing pipeline
		}
		else if (pid == 0)
		{
			// --- Child Process ---
			// process_command used to call handle_child_process, do it directly
			handle_child_process(current_cmd, env, *prev_pipe_read, pipe_fd);
			// handle_child_process exits, so code below won't run in child
		}
		else
		{
			// --- Parent Process ---
			// If this is the last command in the list, store its PID
			if (!current_cmd->next)
			{
				last_pid = pid;
			}
			// Close pipes in the parent
			update_pipe_status(current_cmd, prev_pipe_read, pipe_fd);
		}
		current_cmd = current_cmd->next; // Move to the next command
	}
	if (*prev_pipe_read != -1)
	{
		close(*prev_pipe_read);
		*prev_pipe_read = -1; // Optional: Reset to indicate closed
	}
	// Wait for all children, passing the PID of the last one
	wait_for_children(last_pid);
}

void	cleanup_pipes(int *pipe_fd, int *prev_pipe_read)
{
	if (*prev_pipe_read != -1)
		close(*prev_pipe_read);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
	*prev_pipe_read = pipe_fd[0];
}

void	update_pipe_status(t_cmd *cmd, int *prev_pipe_read, int *pipe_fd)
{
	if (cmd->next)
		cleanup_pipes(pipe_fd, prev_pipe_read);
	else if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
}
#include "inc/minishell.h"

void	handle_sigint_prompt(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130; // Set exit status to 130 for Ctrl-C
}

void	handle_sigint_command(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	g_exit_status = 130; // Set exit status to 130 for Ctrl-C
}

void	setup_signals_for_prompt(void)
{
	signal(SIGINT, handle_sigint_prompt);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signals_for_command(void)
{
	signal(SIGINT, handle_sigint_command);
	signal(SIGQUIT, SIG_IGN);
}
#include "inc/minishell.h"

// Sprawdza, czy znak jest białym znakiem
int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
		|| c == '\v');
}

// Łączy dwa stringi, zwalniając s1
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

// Dodaje pojedynczy znak do stringa
char	*ft_strjoin_char(char *s, char c)
{
	char	*new;
	int		len;

	len = 0;
	if (s)
		len = ft_strlen(s);
	new = malloc(len + 2); // +1 for char, +1 for '\0'
	if (!new)
		return (NULL);
	if (s)
		ft_memcpy(new, s, len);
	new[len] = c;
	new[len + 1] = '\0';
	free(s);
	return (new);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
#include "inc/minishell.h"


/**
 * Łączy trzy stringi w jeden (np. "dir", "/", "cmd" → "dir/cmd").
 * Zwraca nowy string lub NULL przy błędzie alokacji.
 */
char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char *result;
	char *tmp;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
}

void	ft_free_split(char **split)
{
	int i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

/* Helper function to check if command is an existing executable path */
char	*check_absolute_path(char *cmd)
{
	if (!cmd)
		return (NULL);
	// Check if command contains a slash (absolute/relative path)
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
	}
	return (NULL);
}

char	*get_exec_path(char *cmd, t_env *env)
{
	char *path_value;
	char **path_dirs;
	char *exec_path;
	struct stat statbuf; // Declare statbuf here for the explicit path check

	// Basic validation
	if (!cmd || cmd[0] == '\0') // Also check for empty command string
		return (NULL);
	if (!env)          // Need env to check PATH
		return (NULL); // Or decide how to handle no env

	// --- Check if cmd is an explicit path (contains '/') ---
	if (ft_strchr(cmd, '/'))
	{
		// Use stat() to check if the path exists. We don't check permissions here.
		// Let execve handle permissions, directory execution errors etc.
		if (stat(cmd, &statbuf) == 0)
		{
			// Path exists (it could be a file, directory, etc.)
			return (ft_strdup(cmd)); // Return a copy of the path
		}
		else
		{
			// Stat failed (errno is likely ENOENT - No such file or directory)
			return (NULL); // Indicate failure to find the explicit path
		}
	}

	// --- If not an explicit path, search in PATH ---
	path_value = get_env_value("PATH", env);
	if (!path_value || path_value[0] == '\0')
	{
		// PATH not set or empty, cannot search
		return (NULL);
	}

	path_dirs = ft_split(path_value, ':');
	if (!path_dirs)
	{
		// ft_split failed (malloc error likely)
		// Optionally: perror("minishell: ft_split failed for PATH");
		return (NULL);
	}

	// Search through each directory listed in PATH
	exec_path = search_in_path_dirs(cmd, path_dirs);
		// This might return NULL or a malloc'd path

	// Clean up the allocated path directory array
	ft_free_split(path_dirs);

	// Return the result (NULL if not found in PATH,
	//	or the allocated full path)
	return (exec_path);
}

// //moje
char	*search_in_path_dirs(char *cmd, char **dirs)
{
	int i;
	char *full_path;
	const char *dir;

	i = 0;
	while (dirs[i])
	{
		dir = dirs[i];
		if (dir[0] == '\0')
			dir = "."; // Handle empty paths (e.g., PATH=:/bin)
		full_path = ft_strjoin3(dir, "/", cmd);
		if (!full_path)
			continue ;

		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}
