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

void	update_time(t_philo *philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&philo->time_mutex);
	philo->t_current = ((&tv)->tv_sec * 1000) + ((&tv)->tv_usec / 1000);
	philo->ts = philo->t_current - philo->t_start;
	pthread_mutex_unlock(&philo->time_mutex);
}

void	p_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_state(philo, "has taken a fork", C_YEL);
	if (philo->ruleset->philo_nb == 1)
	{
		pthread_mutex_unlock(philo->l_fork);
		usleep(philo->ruleset->t_die * 1000);
		return ;
	}
	pthread_mutex_lock(philo->r_fork);
	print_state(philo, "has taken a fork", C_YEL);
	print_state(philo, "is eating", C_GRN);
	pthread_mutex_lock(&philo->time_mutex);
	philo->t_last_meal = philo->t_current;
	philo->meals_nb++;
	usleep(philo->ut_eat);
	pthread_mutex_unlock(&philo->time_mutex);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	p_sleep(t_philo *philo)
{
	print_state(philo, "is sleeping", C_CYN);
	usleep(philo->ut_sleep);
}

void	p_think(t_philo *philo)
{
	print_state(philo, "is thinking", C_MAG);
}

void	p_init(t_philo *philo, int id, t_rules *ruleset, pthread_mutex_t *fork, t_monitor *monitor)
{
	struct timeval	tv;

	philo->ruleset = ruleset;
	philo->id = id;
	philo->l_fork = fork;
	philo->alive = &monitor->alive;
	philo->alive_mutex = &monitor->alive_mutex;
	philo->meals_eaten = &monitor->meals_eaten;
	philo->meals_mutex = &monitor->meals_mutex;
	philo->print_mutex = &monitor->print_mutex;
	philo->ut_sleep = philo->ruleset->t_sleep * 1000;
	philo->ut_eat = philo->ruleset->t_eat * 1000;
	gettimeofday(&tv, NULL);
	philo->t_start = ((&tv)->tv_sec * 1000) + ((&tv)->tv_usec / 1000);
	philo->t_last_meal = philo->t_start;
	philo->ts = 0;
	philo->meals_nb = 0;
	pthread_mutex_init(&philo->time_mutex, NULL);
}

int	main(int ac, char **av)
{
	t_rules	*ruleset;

	ruleset = (t_rules *)malloc(sizeof(t_rules));
	if (!ruleset)
		return (1);
	if (ac == 5 || ac == 6)
	{
		if (ac == 6 && ft_atoi(av[5]) == 0)
		{
			free(ruleset);
			return (0);
		}
		init_ruleset(ruleset, av);
		create_threads(ruleset);
	}
	else
		printf("Error: Bad number of arguments.");
	free(ruleset);
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int	ft_isdigit(int c)
{
	if (c < '0' || c > '9')
	{
		return (0);
	}
	return (1);
}

int	ft_atoi(const char *nptr)
{
	time_t	i;
	int		sign;
	int		result;

	sign = 1;
	i = 0;
	result = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (ft_isdigit(nptr[i]))
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}

/* TODO : check nb
			si < 0 si > ?
		si time to sleep > time to die error
*/

void	init_ruleset(t_rules *ruleset, char **av)
{
	ruleset->philo_nb = ft_atoi(av[1]);
	ruleset->t_die = ft_atoi(av[2]);
	ruleset->t_eat = ft_atoi(av[3]);
	ruleset->t_sleep = ft_atoi(av[4]);
	if (av[5])
		ruleset->meals_nb = ft_atoi(av[5]);
	else
		ruleset->meals_nb = 0;
}

void	print_state(t_philo *philo, char *action, char *color)
{
	update_time(philo);
	if (!check_status(philo) && ft_strcmp(action, "died"))
		return ;
	pthread_mutex_lock(&philo->time_mutex);
	pthread_mutex_lock(philo->print_mutex);
	printf("%s[%ldms] - %d %s%s\n", color, philo->ts, philo->id, action, C_END);
	pthread_mutex_unlock(&philo->time_mutex);
	pthread_mutex_unlock(philo->print_mutex);
}


int	check_status(t_philo *philo)
{
	pthread_mutex_lock(philo->alive_mutex);
	pthread_mutex_lock(philo->meals_mutex);
	if (!(*philo->alive) || *philo->meals_eaten)
	{
		pthread_mutex_unlock(philo->meals_mutex);
		pthread_mutex_unlock(philo->alive_mutex);
		return (0);
	}
	pthread_mutex_unlock(philo->alive_mutex);
	pthread_mutex_unlock(philo->meals_mutex);
	(void)philo;
	return (1);
}

void	check_if_alive(t_philo *philo)
{
	update_time(philo);
	pthread_mutex_lock(&philo->time_mutex);
	if ((philo->t_current - philo->t_last_meal) >= philo->ruleset->t_die)
	{
		pthread_mutex_unlock(&philo->time_mutex);
		pthread_mutex_lock(philo->alive_mutex);
		*philo->alive = 0;
		pthread_mutex_unlock(philo->alive_mutex);
		print_state(philo, "died", C_RED);
		return ;
	}
	pthread_mutex_unlock(&philo->time_mutex);
}

void	check_if_all_meals_eaten(t_philo *philo)
{
	int i;

	i = 0;
	while (i < philo[0].ruleset->philo_nb)
	{
		if (philo[i].meals_nb < philo[0].ruleset->meals_nb)
			return ;
		i++;
	}
	pthread_mutex_lock(philo->meals_mutex);
	*philo->meals_eaten = 1;
	pthread_mutex_unlock(philo->meals_mutex);
	return ;
}

void	*philo_routine(void *data)
{
	t_philo			*philo;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (check_status(philo))
	{
		p_eat(philo);
		p_sleep(philo);
		p_think(philo);
		usleep(10000);
	}
	return (NULL);
}

void	*monitor_routine(void *data)
{
	t_monitor	*monitor;
	int			i;

	monitor = (t_monitor *)data;
	i = 0;
	while (1)
	{
		if (i == monitor->philo[0].ruleset->philo_nb)
			i = 0;
		check_if_alive(&monitor->philo[i]);
		check_if_all_meals_eaten(monitor->philo);
		if (!monitor->alive || monitor->meals_eaten)
			break ;
		i++;
	}
	return (NULL);
}

void	create_threads(t_rules *ruleset)
{
	int				i;
	t_philo			philo[1024];
	pthread_mutex_t	forks[1024];
	t_monitor		monitor;
	pthread_t		monitor_tid;

	pthread_mutex_init(&monitor.alive_mutex, NULL);
	pthread_mutex_init(&monitor.meals_mutex, NULL);
	pthread_mutex_init(&monitor.print_mutex, NULL);
	monitor.alive = 1;
	monitor.meals_eaten = 0;
	monitor.philo = philo;
	i = 0;
	while (i < ruleset->philo_nb)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < ruleset->philo_nb)
	{
		p_init(&philo[i], (i + 1), ruleset, &forks[i], &monitor);
		if (i == ruleset->philo_nb - 1)
			philo[i].r_fork = &forks[0];
		else
			philo[i].r_fork = &forks[i + 1];
		pthread_create(&philo[i].tid, NULL, philo_routine, &philo[i]);
		i++;
	}
	pthread_create(&monitor_tid, NULL, monitor_routine, &monitor);
	pthread_join(monitor_tid, NULL);
	while (--i >= 0)
	{
		pthread_join(philo[i].tid, NULL);
	}
	while (i < ruleset->philo_nb)
	{
		pthread_mutex_destroy(&forks[i]);
		pthread_mutex_destroy(&philo[i].time_mutex);
		i++;
	}
	pthread_mutex_destroy(&monitor.alive_mutex);
	pthread_mutex_destroy(&monitor.meals_mutex);
	pthread_mutex_destroy(&monitor.print_mutex);
}