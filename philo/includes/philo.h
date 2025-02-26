/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:30:26 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/26 13:35:08 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

# define C_END  "\x1B[0m"
# define C_RED  "\x1B[31m"
# define C_GRN  "\x1B[32m"
# define C_YEL  "\x1B[33m"
# define C_BLU  "\x1B[34m"
# define C_MAG  "\x1B[35m"
# define C_CYN  "\x1B[36m"
# define C_WHT  "\x1B[37m"

typedef struct s_rules
{
	pthread_mutex_t	alive_mutex;
	int				philo_nb;
	int				philo_alive;
	time_t			t_die;
	time_t			t_eat;
	time_t			t_sleep;
	int				meals_nb;
}	t_rules;

typedef struct s_fork
{
	pthread_mutex_t	id_mutex;
	int				id;
}	t_fork;

typedef struct s_philo
{
	pthread_mutex_t	id_mutex;
	int		id;
	pthread_mutex_t	t_start_mutex;
	time_t	t_start;
	pthread_mutex_t	t_current_mutex;
	time_t	t_current;
	pthread_mutex_t	t_last_meal_mutex;
	time_t	t_last_meal;
	pthread_mutex_t	timestamp_mutex;
	time_t	timestamp;
	time_t	ut_sleep;
	time_t	ut_eat;
	t_rules	*ruleset;
	t_fork	*fork;
	pthread_mutex_t	alive_mutex;
	int		alive;
}	t_philo;

void	init_ruleset(t_rules *ruleset, char **av);
void	create_threads(t_rules *ruleset);
void	update_time(struct timeval *tv, t_philo *philo);
int		p_eat(struct timeval *tv, t_philo *philo);
int		p_sleep(struct timeval *tv, t_philo *philo);
int		p_think(struct timeval *tv, t_philo *philo);
void	p_init(struct timeval *tv, t_philo *philo);

#endif