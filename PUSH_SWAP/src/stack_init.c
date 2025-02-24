#include "../../inc/push_swap.h"

static long	ft_atol(const char *s) //Define a function that converts every string into a long value
{
	long	result;
	int		sign;

	result = 0;
	sign = 1; 
	while (*s == ' ' || *s == '\t' || *s == '\n' || \
			*s == '\r' || *s == '\f' || *s == '\v')
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	while (ft_isdigit(*s))
		result = result * 10 + (*s++ - '0');
	return (result * sign);
}

static void	append_node(t_stack_node **stack, int n) //Define a function that searches for the last node to append to the linked list
{
	t_stack_node	*node; 
	t_stack_node	*last_node; 

	if (!stack)
		return ;
	node = malloc(sizeof(t_stack_node)); 
	if (!node)
		return ;
	node->next = NULL; 
	node->nbr = n; 
	node->cheapest = 0; 
	if (!(*stack)) 
	{
		*stack = node; 
	}
	else 
	{
		last_node = find_last(*stack); 
		last_node->next = node; 
		node->prev = last_node; 
	}
}

void	init_stack_a(t_stack_node **a, char **argv) //Define a function that initiates stack `a` by handling any errors and appending required nodes to complete a stack
{
	long	n;
	int		i;

	i = 0;
	while (argv[i])
	{
		if (error_syntax(argv[i]))
			free_errors(a);
		n = ft_atol(argv[i]);
		if (n > INT_MAX || n < INT_MIN) 
			free_errors(a);
		if (error_duplicate(*a, (int)n))
			free_errors(a); 
		append_node(a, (int)n); 
	}
}

t_stack_node	*get_cheapest(t_stack_node *stack) 
{
	if (!stack)
		return (NULL);
	while (stack)
	{
		if (stack->cheapest)
			return (stack);
		stack = stack->next;
	}
	return (NULL);
}

void	prep_for_push(t_stack_node **stack,
						t_stack_node *top_node,
						char stack_name) //Define a function that moves the required node to the top of the stack
{
	while (*stack != top_node) 
	{
		{
			if (top_node->above_median)
				ra(stack, false);
			else
				rra(stack, false);
		}
		else if (stack_name == 'b') 
		{
			if (top_node->above_median)
				rb(stack, false);
			else
				rrb(stack, false);
		}	
	}
}