/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:09:30 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/05/22 18:12:21 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static int	take_fork(pthread_mutex_t *fork, t_philo *philo)
{
	pthread_mutex_lock(fork);
	pthread_mutex_lock(&philo->data->write);
	if (!philo->data->simulation_end)
		message("has taken a fork", philo);
	pthread_mutex_unlock(&philo->data->write);
	return (0);
}

static int	eat_sleep(t_philo *philo)
{
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
	return (0);
}

static void	*single_philo_live(t_philo *philo)
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
	return (NULL);
}

static void	*philo_routine(t_philo *philo)
{
	if (philo->id % 2 == 0)
		ft_usleep(philo, 1);
	while (1)
	{
		if (!check_if_dead(philo))
			return (NULL);
		take_fork(philo->left_fork, philo);
		take_fork(philo->right_fork, philo);
		if (eat_sleep(philo))
			return (NULL);
	}
	return (NULL);
}

void	*live(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->data->num_philo == 1)
		return (single_philo_live(philo));
	return (philo_routine(philo));
}
