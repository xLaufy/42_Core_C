/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:05:09 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/05/22 19:08:13 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	start_threads(t_philo *philo, t_data *data)
{
	int	i;

	data->threads = malloc(sizeof(pthread_t) * data->num_philo * 2);
	if (!data->threads)
		return ;
	i = 0;
	while (i < data->num_philo)
	{
		pthread_create(&data->threads[i], NULL,
			live, (void *)&philo[i]);
		pthread_create(&data->threads[i + data->num_philo], NULL,
			death_monitor, (void *)&philo[i]);
		i++;
	}
}

void	create_philosophers(t_philo *philo, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		philo[i].id = i + 1;
		philo[i].ate = 0;
		philo[i].start_time = get_time();
		philo[i].last_meal = 0;
		philo[i].time_passed = 0;
		if ((i + 1) % 2 == 0)
		{
			philo[i].left_fork = &data->forks[(i + 1) % data->num_philo];
			philo[i].right_fork = &data->forks[i];
		}
		else
		{
			philo[i].left_fork = &data->forks[i];
			philo[i].right_fork = &data->forks[(i + 1) % data->num_philo];
		}
		philo[i].data = data;
		i++;
	}
}

void	init_forks(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philo);
	pthread_mutex_init(&data->write, NULL);
	while (i < data->num_philo)
		pthread_mutex_init(&data->forks[i++], NULL);
}

void	init_data(t_data *data, char **argv, int argc)
{
	data->num_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->simulation_end = 0;
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	data->philo_finish = 0;
}

void	free_all(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->data->num_philo)
		pthread_mutex_destroy(&philo->data->forks[i++]);
	pthread_mutex_destroy(&philo->data->write);
	free(philo->data->forks);
	free(philo->data->threads);
}
