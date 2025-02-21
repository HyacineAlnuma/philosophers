/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:30:26 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/21 12:20:42 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules
{
	int		philo_nb;
	time_t	t_die;
	time_t	t_eat;
	time_t	t_sleep;
	int		meals_nb;
}	t_rules;

typedef struct s_fork
{
	pthread_mutex_t	id_mutex;
	int				id;
}	t_fork;

typedef struct s_philo
{
	int		id;
	int		is_alive;
	time_t	t_start;
	time_t	t_current;
	time_t	t_last_meal;
	time_t	timestamp;
	time_t	ut_sleep;
	time_t	ut_eat;
	t_rules	*ruleset;
	t_fork	*fork;
}	t_philo;

void	init_ruleset(t_rules *ruleset, char **av);
void	create_threads(t_rules *ruleset);
void	update_time(struct timeval *tv, t_philo *philo);
int		p_eat(struct timeval *tv, t_philo *philo);
int		p_sleep(struct timeval *tv, t_philo *philo);
int		p_think(struct timeval *tv, t_philo *philo);
void	p_init(struct timeval *tv, t_philo *philo);

#endif