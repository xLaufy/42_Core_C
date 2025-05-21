#include "philosophers.h"

static void	check_if_finish(t_philo *philo)
{
<<<<<<< HEAD
	if (philo->ate == philo->data->must_eat && philo->data->must_eat != -1)
=======
	if (philo->ate == philo->data->must_eat \
	&& philo->data->must_eat != -1 && philo->ate != -1)
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
	{
		philo->ate = -1;
		philo->data->philo_finish += 1;
	}
<<<<<<< HEAD
	if (philo->data->philo_finish == philo->data->num_philo)
=======
	if (philo->data->num_philo == philo->data->philo_finish)
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
		philo->data->simulation_end = 1;
}

int	check_if_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->write);
	check_if_finish(philo);
<<<<<<< HEAD
	if (philo->data->simulation_end)
=======
	if (philo->data->simulation_end == 1)
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
	{
		pthread_mutex_unlock(&philo->data->write);
		return (0);
	}
<<<<<<< HEAD
	philo->time_passed = (get_time() - philo->start_time) - philo->last_meal;
=======
	philo->time_passed = (get_time() - philo->start_time) \
	- philo->last_meal;
>>>>>>> a3d0c0da23f265b71f20c0097d02291903482ba1
	if (philo->time_passed >= philo->data->time_to_die)
	{
		message("died", philo);
		philo->data->simulation_end = 1;
		pthread_mutex_unlock(&philo->data->write);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->write);
	return (1);
}
