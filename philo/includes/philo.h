/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:30:26 by halnuma           #+#    #+#             */
/*   Updated: 2025/05/05 10:33:10 by halnuma          ###   ########.fr       */
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

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

# define PHILO_MAX  200

typedef pthread_mutex_t	t_mutex;

typedef struct s_rules
{
	int				philo_nb;
	size_t			t_die;
	size_t			t_eat;
	size_t			t_sleep;
	int				meals_nb;
}	t_rules;

typedef struct s_philo
{
	pthread_t		tid;
	int				id;
	int				meals_nb;
	int				*alive;
	int				*meals_eaten;
	size_t			t_start;
	size_t			t_simu_start;
	size_t			t_last_meal;
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
	t_rules			*ruleset;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	alive_mutex;
	int				alive;
	pthread_mutex_t	meals_mutex;
	int				meals_eaten;
}	t_monitor;

// ------ LIBFT ------//

int		ft_atoi(const char *nptr);
int		ft_isdigit(int c);
int		ft_strcmp(const char *s1, const char *s2);
void	ft_putstr_fd(char *s, int fd);

// ------ INIT ------ //

int		init_mtx(t_monitor *monitor, t_mutex *forks, t_rules *ruleset);
int		p_init(t_philo *philo, int id, t_rules *ruleset, size_t t_start);
void	p_init_bis(t_philo *philo, t_mutex *fork, t_monitor *monitor);
void	init_ruleset(t_rules *ruleset, char **av);

// ------ UTILS ------ //

void	destroy_mtx(t_rules *ruleset, t_mutex *forks, t_philo *p, t_monitor *m);
void	print_state(t_philo *philo, char *action, char *color);
int		check_status(t_philo *philo);
void	check_if_alive(t_philo *philo);
void	check_if_all_meals_eaten(t_philo *philo);

// ------ TIME ------ //

size_t	get_current_time(t_philo *philo);
void	custom_usleep(size_t sleep, t_philo *philo);
void	sleep_until_start(t_philo *philo);

// ------ ACTIONS ------ //

void	p_eat(t_philo *philo);
void	p_sleep(t_philo *philo);
void	p_think(t_philo *philo);

// ------ THREADS ------ //

void	create_threads(t_rules *ruleset);

#endif