#include "philosophers.h"

int	ft_usleep(t_philo *philo, size_t milliseconds)
{
<<<<<<< HEAD
	size_t	start = get_time();
=======
	size_t	start;

	start = get_time();
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
	while ((get_time() - start) < milliseconds)
	{
		usleep(500);
		if (check_if_dead(philo) == 0)
			return (1);
	}
	return (0);
}

void	message(char *str, t_philo *philo)
{
	if (philo->data->simulation_end == 0)
<<<<<<< HEAD
		printf("time: [%ld] | Philosopher [%d] %s\n",
			get_time() - philo->start_time, philo->id, str);
=======
		printf("time: [%ld] | Philosopher [%d] %s\n", get_time() - philo->start_time, \
		philo->id, str);
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
}

void	wait_threads(pthread_t *threads, int size)
{
<<<<<<< HEAD
	int	i = 0;
	while (i < size)
		pthread_join(threads[i++], NULL);
=======
	int	i;

	i = 0;
	while (i < size)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
}

long	get_time(void)
{
	struct timeval	time;
<<<<<<< HEAD
=======

>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long	ft_atoi(const char *str)
{
<<<<<<< HEAD
	long	result = 0;
	int		sign = 1;
	int		i = 0;

	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		sign = (str[i++] == '-') ? -1 : 1;
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
=======
	long	result;
	int		sign;
	int		i;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if ((result > 2147483647 || result < -2147483648 || ft_strlen(str) > 11) \
	&& ft_strcmp(str, "-2147483648") != 0)
		return (0);
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
	return (result * sign);
}
