/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:30:26 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/14 10:14:59 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# define C_END  "\x1B[0m"
# define C_RED  "\x1B[31m"
# define C_GRN  "\x1B[32m"
# define C_YEL  "\x1B[33m"
# define C_BLU  "\x1B[34m"
# define C_MAG  "\x1B[35m"
# define C_CYN  "\x1B[36m"
# define C_WHT  "\x1B[37m"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <signal.h>
# include <pthread.h>
# include <fcntl.h>

typedef struct s_rules
{
	int	philo_nb;
	int	t_die;
	int	t_eat;
	int	t_sleep;
	int	meals_nb;
}	t_rules;

typedef struct s_sem
{
	sem_t	*s_forks;
	sem_t	*s_write;
	sem_t	*s_meals;
}	t_sem;

typedef struct s_philo
{
	pid_t	*pid;
	int		id;
	int		meals_nb;
	int		*alive;
	int		*meals_eaten;
	time_t	t_start;
	time_t	t_current;
	time_t	t_last_meal;
	time_t	ts;
	time_t	ut_sleep;
	time_t	ut_eat;
	t_rules	*ruleset;
	t_sem	*sems;
}	t_philo;

typedef struct s_monitor
{
	pid_t	pids[1024];
	t_philo	*philo;
	int		alive;
	int		meals_eaten;
}	t_monitor;

void	init_ruleset(t_rules *ruleset, char **av);
void	create_processes(t_rules *ruleset);
void	p_eat(t_philo *philo);
void	p_sleep(t_philo *philo);
void	p_think(t_philo *philo);
void	p_init(t_philo *philo, int id, t_rules *ruleset, t_monitor *monitor);
void	update_time(t_philo *philo);
void	print_state(t_philo *philo, char *action, char *color);
int		check_status(t_philo *philo);

#endif