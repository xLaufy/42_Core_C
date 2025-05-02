#include "../inc/minishell.h"
//////////start moje

///moje
// int	create_heredoc_pipe(int pipe_fd[2])
// {
// 	if (pipe(pipe_fd) == -1)
// 	{
// 		perror("pipe");
// 		return (-1);
// 	}
// 	return (0);
// }



int create_heredoc_pipe(int pipe_fd[2])
{
    if (pipe(pipe_fd) == -1)
    {
        perror("minishell: pipe");
        return (-1);
    }
    return (0);
}




void	process_heredoc_line(int write_fd, char *line)
{
	// Write to pipe (with newline)
	write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
	free(line);
}


// //moje
// int	handle_heredoc(const char *delimiter)
// {
// 	char	*line;
// 	int		pipe_fd[2];

// 	line = NULL;
// 	if (create_heredoc_pipe(pipe_fd) < 0)
// 		return (-1);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line) // Handle Ctrl+D
// 		{
// 			write(1, "\n", 1);
// 			break ;
// 		}
// 		if (ft_strcmp(line, delimiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		process_heredoc_line(pipe_fd[1], line);
// 	}
// 	close(pipe_fd[1]);
// 	return (pipe_fd[0]);
// }
int handle_heredoc(char *delimiter)
{
    char *line;
    int pipe_fd[2];
    int return_fd = -1;
    bool delimiter_found = false;

    if (create_heredoc_pipe(pipe_fd) < 0) // create_heredoc_pipe just calls pipe()
        return (-1);

    return_fd = pipe_fd[0]; // Store the read end

    // Optional: Temporarily ignore SIGINT for heredoc input? Complex.
    // Setup signals appropriate for interactive input if needed.

    while (1)
    {
        line = readline("> ");

        if (!line) // Handle Ctrl+D (EOF) or readline error
        {
            ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
            ft_putstr_fd(delimiter, STDERR_FILENO);
            ft_putstr_fd("')\n", STDERR_FILENO);
            break; // Exit loop - unsuccessful termination
        }

        if (ft_strcmp(line, delimiter) == 0)
        {
            delimiter_found = true; // Mark as successful
            free(line);
            break; // Exit loop - successful termination
        }
        // Write line + newline to the pipe
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }

    close(pipe_fd[1]); // Close write end

    if (delimiter_found) {
        return (return_fd); // Return the read FD on success
    } else {
        close(return_fd); // Close the read end as it's not needed/valid
        return (-1); // Return an error code
    }
}

int	handle_heredoc_redir(char *delimiter)
{
	int	fd;

	fd = handle_heredoc(delimiter); // Call heredoc handler
	if (fd < 0)
		return (-1);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

////////////koniec moje

//////////////////////ponizej nowe

int handle_all_heredocs(t_cmd *cmds)
{
    t_cmd   *current_cmd;
    t_redir *current_redir;
    int     fd;

    current_cmd = cmds;
    while (current_cmd)
    {
        current_redir = current_cmd->redirections;
        while (current_redir)
        {
            if (current_redir->type == T_HEREDOC)
            {
                // Make sure heredoc_fd is reset (important if function called multiple times, though unlikely here)
                if (current_redir->heredoc_fd != -1) {
                   close(current_redir->heredoc_fd);
                   current_redir->heredoc_fd = -1;
                }

                // 'file' field holds the delimiter for T_HEREDOC
                fd = handle_heredoc(current_redir->file);

                if (fd < 0)
                {
                    // handle_heredoc should print the warning on EOF
                    g_exit_status = 1; // Set error status
                    // No need to close FDs here, as handle_heredoc closes on error
                    return (-1); // Signal failure
                }
                current_redir->heredoc_fd = fd; // Store the read end FD
            }
            current_redir = current_redir->next;
        }
        current_cmd = current_cmd->next;
    }
    return (0); // Success
}

void close_heredoc_fds(t_cmd *cmds)
{
    t_cmd   *current_cmd;
    t_redir *current_redir;

    current_cmd = cmds;
    while (current_cmd)
    {
        current_redir = current_cmd->redirections;
        while (current_redir)
        {
            if (current_redir->type == T_HEREDOC && current_redir->heredoc_fd != -1)
            {
                close(current_redir->heredoc_fd);
                current_redir->heredoc_fd = -1; // Mark as closed
            }
            current_redir = current_redir->next;
        }
        current_cmd = current_cmd->next;
    }
}
#include "minishell.h"

// dup - tworzy kopie file descriptora oldfd, using the lowest-numbered
// unused file descriptor fot the new descriptor
// dup2
//	- to samo tylko uzywa konkretnego file descriptora podanego mu do funkcji
// int dup2(int oldfd, int newfd);

// < file
// // zamiast czytac z klawiatury (stdin), czytaj z tego file
int	handle_input_redir(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (-1); // Do not print error message here
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_output_redir(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (-1); // Do not print error message here
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_append_redir(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (-1); // Do not print error message here
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
#include "../inc/minishell.h"

void	restore_std_fds(int stdin_copy, int stdout_copy)
{
	// Ensure descriptors are valid before dup2
	if (stdin_copy >= 0)
	{
		dup2(stdin_copy, STDIN_FILENO);
		close(stdin_copy);
	}
	if (stdout_copy >= 0)
	{
		dup2(stdout_copy, STDOUT_FILENO);
		close(stdout_copy);
	}
}
// ////dziala dla naszych testow ale nie heredoc
// //moje
// int	process_single_redirection(t_redir *redir)
// {
// 	int	status;

// 	int saved_errno; // Variable to store errno immediately after failure
// 	// Check for missing filename (syntax error)
// 	if (!redir || !redir->file || redir->file[0] == '\0')
// 	{
// 		ft_putstr_fd("minishell: syntax error: "
// 						"missing filename after redirection\n",
// 						STDERR_FILENO);
// 		g_exit_status = 2; // Syntax errors often have exit code 2 in bash
// 		return (-1);
// 	}
// 	status = 0; // Initialize status
// 	// --- Perform the specific redirection ---
// 	// These handle_* functions should return -1 on error and set errno.
// 	if (redir->type == T_REDIR_IN)
// 		status = handle_input_redir(redir->file);
// 	else if (redir->type == T_REDIR_OUT)
// 		status = handle_output_redir(redir->file);
// 	else if (redir->type == T_APPEND)
// 		status = handle_append_redir(redir->file);
// 	else if (redir->type == T_HEREDOC)
// 		status = handle_heredoc_redir(redir->file);
// 	// Add else if for other types if necessary
// 	// --- Check for errors and print appropriate message ---
// 	if (status < 0)
// 	{
// 		saved_errno = errno; // Save errno immediately after open/dup2 failure
// 		ft_putstr_fd("minishell: ", STDERR_FILENO);
// 		ft_putstr_fd(redir->file, STDERR_FILENO);
// 		ft_putstr_fd(": ", STDERR_FILENO);
// 		// Print the actual system error message
// 		ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
// 		ft_putstr_fd("\n", STDERR_FILENO);
// 		g_exit_status = 1; // Redirection errors typically yield exit code 1
// 		return (-1);       // Return error
// 	}
// 	return (0); // Success
// }

int process_single_redirection(t_redir *redir)
{
    int status = 0; // Initialize status to success
    int saved_errno;

    if (!redir || !redir->file || redir->file[0] == '\0') {
        // This check is particularly important for non-heredoc cases
        // For heredoc, redir->file is the delimiter and should exist
        ft_putstr_fd("minishell: syntax error: missing filename or delimiter\n", STDERR_FILENO);
        g_exit_status = 2;
        return (-1);
    }

    // --- Perform the specific redirection ---
    if (redir->type == T_REDIR_IN)
        status = handle_input_redir(redir->file);
    else if (redir->type == T_REDIR_OUT)
        status = handle_output_redir(redir->file);
    else if (redir->type == T_APPEND)
        status = handle_append_redir(redir->file);
    else if (redir->type == T_HEREDOC)
    {
        // Heredoc already processed, use the stored FD
        if (redir->heredoc_fd < 0) {
            // Should not happen if handle_all_heredocs succeeded
            ft_putstr_fd("minishell: internal error: invalid heredoc fd for ", STDERR_FILENO);
             ft_putstr_fd(redir->file, STDERR_FILENO); // Print delimiter for context
             ft_putstr_fd("\n", STDERR_FILENO);
            status = -1; // Signal internal error
        } else {
             // Duplicate the read end of the pipe to standard input
            if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0) {
                 status = -1; // dup2 failed, errno is set
            }
             // *** CRUCIAL ***: Close the original heredoc FD in the child
             //                 after dup2. We don't need it anymore.
            close(redir->heredoc_fd);
        }
    }
    // Add else if for other types if necessary

    // --- Check for errors and print appropriate message ---
    if (status < 0)
    {
        saved_errno = errno; // Save errno immediately after failure
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        // For T_HEREDOC error (likely dup2 fail), print delimiter as context
        // For others, print filename.
        ft_putstr_fd(redir->file, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putstr_fd(strerror(saved_errno), STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
        g_exit_status = 1;
        return (-1); // Return error
    }

    return (0); // Success
}


// moje, nie dziala na podwojego heredoca
/* Main function that processes all redirections in the list */
int	setup_redirections(t_redir *redir_list)
{
	while (redir_list)
	{
		if (process_single_redirection(redir_list) < 0)
			return (-1);
		redir_list = redir_list->next;
	}
	return (0);
}


// // //with debugging steps
// int	setup_redirections(t_redir *redir_list)
// {
// 	int	status;

// 	printf("DEBUG: Entering setup_redirections\n");
// 	while (redir_list)
// 	{
// 		printf("DEBUG: Processing redirection type %d, file/delim '%s'\n",
// 			redir_list->type, redir_list->file);
// 		status = process_single_redirection(redir_list);
// 		printf("DEBUG: process_single_redirection returned %d\n", status);
// 		if (status < 0)
// 		{
// 			printf("DEBUG: Error detected, returning -1\n");
// 			return (-1);
// 		}
// 		redir_list = redir_list->next;
// 	}
// 	printf("DEBUG: Exiting setup_redirections successfully\n");
// 	return (0);
// }


// minishell$ cat <<HERE <<DOC
// DEBUG: Entering setup_redirections
// DEBUG: Processing redirection type 5, file/delim 'HERE'  // First heredoc
// > oi                                                    // Input for HERE
// > HERE                                                  // Delimiter for HERE
// DEBUG: process_single_redirection returned 0            // First heredoc OK
// DEBUG: Processing redirection type 5, file/delim 'DOC'  // Second heredoc
// > oi                                                    // Input for DOC (You typed 'oi' again)
// >                                                       // Input for DOC (You typed ENTER - empty line)
// DEBUG: process_single_redirection returned 0            // Second heredoc *claims* OK (Incorrect behavior!)
// DEBUG: Exiting setup_redirections successfully
// oi                                                      // Output of cat
// minishell$