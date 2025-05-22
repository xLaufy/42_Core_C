/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkrawczy <mkrawczy@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:05:15 by mkrawczy          #+#    #+#             */
/*   Updated: 2025/05/22 18:05:42 by mkrawczy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	int				num_philo;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				must_eat;
	int				philo_finish;
	int				simulation_end;
	pthread_mutex_t	write;
	pthread_mutex_t	*forks;
	pthread_t		*threads;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				ate;
	long			start_time;
	long			last_meal;
	long			time_passed;
	t_data			*data;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}					t_philo;

void				*live(void *data);
int					check_if_dead(t_philo *philo);

void				init_data(t_data *data, char **argv, int argc);
void				init_forks(t_data *data);
void				create_philosophers(t_philo *philo, t_data *data);
void				start_threads(t_philo *philo, t_data *data);
void				free_all(t_philo *philo);

long				ft_atoi(const char *str);
int					ft_strlen(const char *str);
int					ft_strcmp(const char *str1, const char *str2);
int					check_argv(int argc, char **argv);
void				wait_threads(pthread_t *threads, int size);
long				get_time(void);
void				message(char *str, t_philo *philo);
int					ft_usleep(t_philo *philo, size_t milliseconds);
int					basic_argcheck(int argc, char **argv);
#endif
