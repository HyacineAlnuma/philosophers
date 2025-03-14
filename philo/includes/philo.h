/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:30:26 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/14 17:35:23 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define C_END  "\x1B[0m"
# define C_RED  "\x1B[31m"
# define C_GRN  "\x1B[32m"
# define C_YEL  "\x1B[33m"
# define C_BLU  "\x1B[34m"
# define C_MAG  "\x1B[35m"
# define C_CYN  "\x1B[36m"
# define C_WHT  "\x1B[37m"

# define PHILO_MAX  500

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules
{
	int				philo_nb;
	time_t			t_die;
	time_t			t_eat;
	time_t			t_sleep;
	int				meals_nb;
}	t_rules;

typedef struct s_philo
{
	pthread_t		tid;
	int				id;
	int				meals_nb;
	int				*alive;
	int				*meals_eaten;
	time_t			t_start;
	time_t			t_current;
	time_t			t_last_meal;
	time_t			ts;
	time_t			ut_sleep;
	time_t			ut_eat;
	t_rules			*ruleset;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*alive_mutex;
	pthread_mutex_t	time_mutex;
	pthread_mutex_t	*print_mutex;
	pthread_mutex_t	*meals_mutex;
}	t_philo;

typedef struct s_monitor
{
	t_philo			*philo;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	alive_mutex;
	int				alive;
	pthread_mutex_t	meals_mutex;
	int				meals_eaten;
}	t_monitor;

void	init_ruleset(t_rules *ruleset, char **av);
void	create_threads(t_rules *ruleset);
void	update_time(t_philo *philo);
void	p_eat(t_philo *philo);
void	p_sleep(t_philo *philo);
void	p_think(t_philo *philo);
void	p_init(t_philo *philo, int id, t_rules *ruleset, pthread_mutex_t *fork, t_monitor *monitor);
int		check_status(t_philo *philo);
int		ft_atoi(const char *nptr);
void	print_state(t_philo *philo, char *action, char *color);
size_t	get_current_time(void);

#endif