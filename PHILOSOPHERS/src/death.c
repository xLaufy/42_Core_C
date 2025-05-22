/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:05:04 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/05/22 18:12:21 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

static void	check_if_finish(t_philo *philo)
{
	if (philo->ate == philo->data->must_eat && philo->data->must_eat != -1)
	{
		philo->ate = -1;
		philo->data->philo_finish += 1;
	}
	if (philo->data->philo_finish == philo->data->num_philo)
		philo->data->simulation_end = 1;
}

int	check_if_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->write);
	check_if_finish(philo);
	if (philo->data->simulation_end)
	{
		pthread_mutex_unlock(&philo->data->write);
		return (0);
	}
	philo->time_passed = (get_time() - philo->start_time) - philo->last_meal;
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
