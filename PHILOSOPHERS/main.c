#include "philosophers.h"

<<<<<<< HEAD
static int	take_fork(pthread_mutex_t *fork, t_philo *philo)
{
	pthread_mutex_lock(fork);
	pthread_mutex_lock(&philo->data->write);
	if (!philo->data->simulation_end)
		message("has taken a fork", philo);
	pthread_mutex_unlock(&philo->data->write);
	return (0);
=======
static int	left_fork(t_philo *philo)
{
	int	simulation_end;

	simulation_end = 0;
	while (simulation_end == 0)
	{
		if (philo->data->forks_state[philo->id - 1] == 1)
		{
			if (check_if_dead(philo) == 0)
				simulation_end = 1;
		}
		else
		{
			pthread_mutex_lock(philo->left_fork);
			philo->data->forks_state[philo->id - 1] = 1;
			pthread_mutex_lock(&philo->data->write);
			message("has taken a fork", philo);
			pthread_mutex_unlock(&philo->data->write);
			return (0);
		}
	}
	return (1);
}

static int	right_fork(t_philo *philo)
{
	int	fork_index;

	fork_index = philo->id;
	if (philo->id == philo->data->num_philo)
		fork_index = 0;
	while (philo->data->simulation_end == 0)
	{
		if (philo->data->forks_state[fork_index] == 1)
			check_if_dead(philo);
		else
		{
			pthread_mutex_lock(philo->right_fork);
			philo->data->forks_state[fork_index] = 1;
			pthread_mutex_lock(&philo->data->write);
			message("has taken a fork", philo);
			pthread_mutex_unlock(&philo->data->write);
			return (0);
		}
	}
	pthread_mutex_unlock(philo->left_fork);
	philo->data->forks_state[philo->id - 1] = 0;
	return (1);
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
}

static int	eat_sleep(t_philo *philo)
{
<<<<<<< HEAD
	pthread_mutex_lock(&philo->data->write);
	if (philo->ate != -1)
		philo->ate += 1;
	message("is eating", philo);
	philo->last_meal = get_time() - philo->start_time;
	pthread_mutex_unlock(&philo->data->write);

	ft_usleep(philo, philo->data->time_to_eat);

	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);

	pthread_mutex_lock(&philo->data->write);
	message("is sleeping", philo);
	pthread_mutex_unlock(&philo->data->write);

	if (ft_usleep(philo, philo->data->time_to_sleep) == 1)
		return (1);

	pthread_mutex_lock(&philo->data->write);
	message("is thinking", philo);
	pthread_mutex_unlock(&philo->data->write);

=======
	if (philo->ate != -1)
		philo->ate += 1;
	pthread_mutex_lock(&philo->data->write);
	message("is eating", philo);
	philo->last_meal = get_time() - philo->start_time;
	pthread_mutex_unlock(&philo->data->write);
	ft_usleep(philo, philo->data->time_to_eat);
	if (philo->id == philo->data->num_philo)
		philo->data->forks_state[0] = 0;
	else
		philo->data->forks_state[philo->id] = 0;
	pthread_mutex_unlock(philo->right_fork);
	philo->data->forks_state[philo->id - 1] = 0;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_lock(&philo->data->write);
	message("is sleeping", philo);
	pthread_mutex_unlock(&philo->data->write);
	if (ft_usleep(philo, philo->data->time_to_sleep) == 1)
		return (1);
	pthread_mutex_lock(&philo->data->write);
	message("is thinking", philo);
	pthread_mutex_unlock(&philo->data->write);
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
	return (0);
}

void	*live(void *data)
{
<<<<<<< HEAD
	t_philo	*philo = (t_philo *)data;

	if (philo->data->num_philo == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->data->write);
		message("has taken a fork", philo);
		pthread_mutex_unlock(&philo->data->write);
		ft_usleep(philo, philo->data->time_to_die);
		pthread_mutex_lock(&philo->data->write);
		message("died", philo);
		philo->data->simulation_end = 1;
		pthread_mutex_unlock(&philo->data->write);
		pthread_mutex_unlock(philo->left_fork);
		return NULL;
	}

	if (philo->id % 2 == 0)
		ft_usleep(philo, 1);

	while (1)
	{
		if (check_if_dead(philo) == 0)
			return (NULL);
		take_fork(philo->left_fork, philo);
		take_fork(philo->right_fork, philo);
=======
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		ft_usleep(philo, 1);
	while (1)
	{
		if (left_fork(philo) == 1)
			return (NULL);
		if (right_fork(philo) == 1)
			return (NULL);
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
		if (eat_sleep(philo) == 1)
			return (NULL);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
<<<<<<< HEAD
	t_data	data;
	t_philo	*philo;
=======
	t_data			data;
	t_philo			*philo;
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1

	if (check_argv(argc, argv) == 1)
		return (1);
	init_data(&data, argv, argc);
	init_forks(&data);
	philo = malloc(sizeof(t_philo) * data.num_philo);
<<<<<<< HEAD
	if (!philo)
=======
	if (philo == NULL)
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
		return (1);
	create_philosophers(philo, &data);
	start_threads(philo, &data);
	wait_threads(data.threads, data.num_philo);
	free_all(philo);
	free(philo);
}
