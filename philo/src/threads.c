#include "philo.h"

void	check_if_alive(struct timeval *tv, t_philo *philo)
{
	update_time(tv, philo);
	if ((philo->t_current - philo->t_last_meal) > philo->ruleset->t_die)
	{
		printf("%s[%ldms] - %d died%s\n", C_RED, philo->timestamp, philo->id, C_END);
		philo->is_alive = 0;
	}
}

int	try_to_eat(struct timeval *tv, t_philo *philo)
{
	int	right_fork;
	int	left_fork;
	// int	m_right_fork;
	// int	m_left_fork;

	left_fork = philo->id;
	right_fork = philo->id + 1;
	// m_left_fork = 1;
	// m_left_fork = 1;
	// if (philo->id == philo->ruleset->philo_nb)
	// 	right_fork = 1;
	// while (1)
	// {
	// 	if (m_left_fork)
	// 		m_left_fork = pthread_mutex_lock(&philo->fork[left_fork].id_mutex);
	// 	if (m_left_fork)
	// 		m_right_fork = pthread_mutex_lock(&philo->fork[right_fork].id_mutex);
	// 	if (!m_left_fork && !m_right_fork)
	// 		break ;
	// 	check_if_alive(tv, philo);
	// 	if (!philo->is_alive)
	// 		return (0);
	// 	usleep(9000);		
	// }
	pthread_mutex_lock(&philo->fork[left_fork].id_mutex);
	check_if_alive(tv, philo);
	if (!philo->is_alive)
		return (0);
	//printf("PHILO:%d  as taken LEFT FORK:%d\n", philo->id, philo->fork[left_fork].id);
	pthread_mutex_lock(&philo->fork[right_fork].id_mutex);
	check_if_alive(tv, philo);
	if (!philo->is_alive)
		return (0);
	//printf("PHILO:%d  as taken RIGHT FORK:%d\n", philo->id, philo->fork[right_fork].id);
	p_eat(tv, philo);
	pthread_mutex_unlock(&philo->fork[left_fork].id_mutex);
	pthread_mutex_unlock(&philo->fork[right_fork].id_mutex);
	return (1);
}

void	p_live(struct timeval *tv, t_philo *philo)
{
	// int i = 0;
	// (void)tv;
	// while (i < 3)
	// {
	// 	pthread_mutex_lock(&philo->fork[i + 1].id_mutex);
	// 	printf("PHILO:%d  FORK:%d\n", philo->id, philo->fork[i + 1].id);
	// 	pthread_mutex_unlock(&philo->fork[i + 1].id_mutex);
	// 	i++;
	// }


	// while (1)
	// {
	// 	while (1)
	// 	{
	// 		if (try_to_eat(tv, philo))
	// 			break ;
	// 		update_time(tv, philo);
	// 		if ((philo->t_current - philo->t_last_meal) > philo->ruleset->t_die)
	// 		{
	// 			printf("[%ld] - %d died\n", philo->timestamp, philo->id);
	// 			philo->is_alive = 0;
	// 			break ;
	// 		}
	// 		usleep(150);
	// 	}
	// 	if (!philo->is_alive)
	// 		break ;
	// 	p_sleep(tv, philo);
	// 	p_think(tv, philo);
	// }

	while (1)
	{
		if (!try_to_eat(tv, philo))
			break ;
		p_sleep(tv, philo);
		p_think(tv, philo);
	}
}

void	*start_routine(void *data)
{
	struct timeval	tv;
	t_philo			*philo;

	philo = (t_philo *)data;
	p_init(&tv, philo);
	p_live(&tv, philo);
	return (NULL);
}

void	create_threads(t_rules *ruleset)
{
	int			i;
	pthread_t	tid[1024];
	t_philo		philo[1024];
	t_fork		fork[1024];

	i = -1;
	while (++i < ruleset->philo_nb)
	{
		fork[i + 1].id = i + 1;
		pthread_mutex_init(&fork[i + 1].id_mutex, NULL);
		philo[i].ruleset = ruleset;
		philo[i].id = i + 1;
		philo[i].fork = fork;
		pthread_create(&tid[i], NULL, start_routine, &philo[i]);
	}
	while (--i >= 0)
	{
		pthread_mutex_destroy(&fork[i + 1].id_mutex);
		pthread_join(tid[i], NULL);
	}
}
