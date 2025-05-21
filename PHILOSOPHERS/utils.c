#include "philosophers.h"

int	ft_usleep(t_philo *philo, size_t milliseconds)
{
	size_t	start = get_time();
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
		printf("time: [%ld] | Philosopher [%d] %s\n",
			get_time() - philo->start_time, philo->id, str);
}

void	wait_threads(pthread_t *threads, int size)
{
	int	i = 0;
	while (i < size)
		pthread_join(threads[i++], NULL);
}

long	get_time(void)
{
	struct timeval	time;
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long	ft_atoi(const char *str)
{
	long	result = 0;
	int		sign = 1;
	int		i = 0;

	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
		sign = (str[i++] == '-') ? -1 : 1;
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
	return (result * sign);
}
