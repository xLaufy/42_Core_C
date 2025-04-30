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

void	handle_eof(int exit_status)
{
	// Don't free env here, it's already freed in cleanup
	// free_env(env);  <- Remove this line
	// Clear readline history
	rl_clear_history();
	// Print a newline for better UX when exiting with Ctrl+D
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(exit_status);
}

void	process_input(char *input, t_env **env)
{
	t_token	*tokens;
	t_cmd	*cmds;
	t_fds	fds;

	tokens = NULL;
	cmds = NULL;
	t_resources res = {0}; // Initialize resources structure
	// Track ALL resources from the beginning
	res.input = input;
	res.env = *env;
	tokens = tokenize_input(input);
	if (!tokens)
	{
		free(input);
		return ;
	}
	res.tokens = tokens; // Save tokens in resources
	expand_variables(tokens, *env);
	cmds = parse_tokens(tokens);
	if (!cmds)
	{
		free_tokens(tokens);
		free(input);
		return ;
	}
	res.cmds = cmds; // Save commands in resources
	/// poniej
	if (cmds->args && cmds->args[0])
	{
		setup_signals_for_command(); // Set handlers for command execution
		if (is_builtin(cmds->args[0]) && !cmds->next)
		{
			fds.stdin = dup(STDIN_FILENO); // Backup std fds
			fds.stdout = dup(STDOUT_FILENO);
			handle_parent_builtin(cmds, fds, &res);
		}
		else
		{
			execute(cmds, env);
			// This function contains execute_pipeline->wait_for_children
		}
		setup_signals_for_prompt(); // Reset handlers for the next prompt
	}
	// Only clean up if we didn't call exit
	free_cmds(cmds);
	free_tokens(tokens);
	free(input);
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

///////////////////////////nowa proba

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

// //dialalo,
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

int handle_quoted_append(char *input, int *i, char **current_word, int *error, t_parse_state *state) {
    t_quote current_quote = get_quote_type(input[*i]);
    char quote_char = (current_quote == SINGLE_QUOTE) ? '\'' : '"';
    int start;

    (*i)++;
    start = *i;
    while (input[*i] && input[*i] != quote_char) {
        (*i)++;
    }
    if (input[*i] != quote_char) {
        ft_putstr_fd("Minishell: syntax error: unclosed quote\n", STDERR_FILENO);
        *error = 1;
        return 1;
    }

    // Ustawiamy flagę, jeśli napotkaliśmy pojedynczy cudzysłów
    if (current_quote == SINGLE_QUOTE) {
        state->single_quote_used = 1;
    }

    if (!ensure_current_word(current_word)) {
        *error = 1;
        return 1;
    }

    int len = *i - start;
    if (len > 0) {
        char *content = ft_substr(input, start, len);
        if (!content) {
            perror("minishell: malloc error");
            *error = 1; return 1;
        }
        char *old_word = *current_word;
        *current_word = ft_strjoin(old_word, content);
        free(old_word);
        free(content);
        if (!*current_word) {
             perror("minishell: malloc error");
            *error = 1; return 1;
        }
    }

    (*i)++;
    return 0;
}



void finalize_current_word(t_token **tokens, char **current_word, int single_quote_used) {
    if (*current_word != NULL) {
        t_token_type type = single_quote_used ? T_SINGLE_QUOTED : T_WORD;
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

// //dialalo
void parse_input_loop(char *input, t_parse_state *state) {
    while (input[state->i] && !(state->error)) {
        if (ft_isspace(input[state->i])) {
            // Finalizuj jakiekolwiek słowo przed pominięciem spacji
            finalize_current_word(&state->tokens, &state->current_word, state->single_quote_used);
            state->i++; // Pomijamy spację
        } else if (is_pipe(input[state->i])) {
            // Finalizuj słowo, dodaj token dla pipe
            finalize_current_word(&state->tokens, &state->current_word, state->single_quote_used);
            add_token(&state->tokens, "|", T_PIPE);
            state->i++;
        } else if (is_redirect(input[state->i])) {
            // Finalizuj słowo przed obsługą operatora przekierowania
            finalize_current_word(&state->tokens, &state->current_word, state->single_quote_used);
            handle_redirection(input, &state->i, &state->tokens); // Dodaje token przekierowania
        } else if (is_quote(input[state->i])) {
            // Dodaj zawartość cytatu do aktualnie budowanego słowa
            if (handle_quoted_append(input, &state->i, &state->current_word, &state->error, state) != 0) {
                // Wystąpił błąd (niezamknięty cudzysłów lub błąd malloc)
                state->error = 1; // Ustawiamy flagę błędu
            }
        } else {
            // Dodaj zwykły znak do aktualnie budowanego słowa
            append_char_to_word(&state->current_word, input[state->i]);
             if (state->current_word == NULL && errno == ENOMEM) { // Sprawdzamy, czy append_char się nie udał
                 state->error = 1;
             }
            state->i++;
        }
    }

    // Po zakończeniu pętli, finalizuj jakiekolwiek pozostałe słowo
    if (!state->error) { // Finalizujemy tylko, jeśli nie wystąpił błąd w trakcie pętli
         finalize_current_word(&state->tokens, &state->current_word, state->single_quote_used);
    }
}



// //diala
t_token *tokenize_input(char *input)
{
    t_parse_state state;

    state.tokens = NULL;
    state.current_word = NULL;
    state.i = 0;
    state.error = 0;
    state.single_quote_used = 0; // Inicjalizujemy na 0

    parse_input_loop(input, &state);

    // Finalne słowo obsługiwane *wewnątrz* i *po* parse_input_loop

    if (state.error) {
        // Czyszczenie potencjalnie częściowo zbudowanego słowa i tokenów
        if (state.current_word) free(state.current_word);
        free_tokens(state.tokens);
        return (NULL);
    }
    return (reverse_tokens(state.tokens));
}


