/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:30:26 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/02 12:49:48 by halnuma          ###   ########.fr       */
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

# define PHILO_MAX  200

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
# include <limits.h>

typedef struct s_rules
{
	int		philo_nb;
	size_t	t_die;
	size_t	t_eat;
	size_t	t_sleep;
	int		meals_nb;
}	t_rules;

typedef struct s_sem
{
	sem_t	*s_forks;
	sem_t	*s_write;
	sem_t	*s_meals;
	sem_t	*s_death;
}	t_sem;

typedef struct s_philo
{
	pid_t	*pid;
	int		id;
	int		meals_nb;
	size_t	t_start;
	size_t	t_current;
	size_t	t_last_meal;
	size_t	ts;
	size_t	ut_sleep;
	size_t	ut_eat;
	t_rules	*ruleset;
	t_sem	*sems;
}	t_philo;

typedef struct s_monitor
{
	pthread_mutex_t	m_pid;
	pid_t			pids[PHILO_MAX];
	t_philo			*philo;
	t_sem			*sems;
	t_rules			*ruleset;
}	t_monitor;

typedef struct s_death
{
	t_monitor	*monitor;
	int			index;
}	t_death;

// Libft
int		ft_atoi(const char *nptr);
int		ft_isdigit(int c);
int		ft_strcmp(const char *s1, const char *s2);
void	ft_putstr_fd(char *s, int fd);

//Init
void	p_init(t_philo *philo, int id, t_rules *ruleset);
void	init_ruleset(t_rules *ruleset, char **av);
int		init_monitor(t_monitor *m, t_philo *philo, t_rules *r, t_sem *sems);
int		init_sems(t_sem *sems, t_rules *ruleset);

//Utils
void	print_state(t_philo *philo, char *action, char *color);
void	*death_checker(void *data);

//Time
size_t	get_current_time(void);
void	update_time(t_philo *philo);

//Actions
void	p_eat(t_philo *philo);
void	p_sleep(t_philo *philo);
void	p_think(t_philo *philo);

//Process
void	create_processes(t_rules *ruleset);

//Exit
void	close_sems(t_sem *sems);
void	kill_all_philos(t_monitor *monitor, int bool);

#endif