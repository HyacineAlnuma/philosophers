/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:40:08 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/25 12:57:23 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *data)
{
	t_philo			*philo;

	philo = (t_philo *)data;
	// update_time(philo);
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		p_eat(philo);
		p_sleep(philo);
		p_think(philo);
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
		if (monitor->philo[0].ruleset->meals_nb)
			check_if_all_meals_eaten(monitor->philo);
		pthread_mutex_lock(&(monitor->alive_mutex));
		pthread_mutex_lock(&(monitor->meals_mutex));
		if (!monitor->alive || monitor->meals_eaten)
			break ;
		pthread_mutex_lock(&(monitor->alive_mutex));
		pthread_mutex_lock(&(monitor->meals_mutex));
		i++;
	}
	return (NULL);
}

int	thread_monitor(t_monitor *monitor, pthread_t *monitor_tid, t_philo *philo)
{
	monitor->alive = 1;
	monitor->meals_eaten = 0;
	monitor->philo = philo;
	if (pthread_create(monitor_tid, NULL, monitor_routine, monitor))
	{
		ft_putstr_fd("Error: thread creation failed.\n", 2);
		return (0);
	}
	pthread_join(*monitor_tid, NULL);
	return (1);
}

int	launch_philos(t_rules *ruleset, t_monitor *monitor, t_philo *p, t_mutex *f)
{
	int	i;

	i = 0;
	while (i < ruleset->philo_nb)
	{
		if (!p_init(&p[i], (i + 1), ruleset))
			return (0);
		p_init_bis(&p[i], &f[i], monitor);
		if (i == ruleset->philo_nb - 1)
			p[i].r_fork = &f[0];
		else
			p[i].r_fork = &f[i + 1];
		if (pthread_create(&p[i].tid, NULL, philo_routine, &p[i]))
		{
			ft_putstr_fd("Error: thread creation failed.\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

void	create_threads(t_rules *ruleset)
{
	int			i;
	t_philo		philo[PHILO_MAX];
	t_mutex		forks[PHILO_MAX];
	t_monitor	monitor;
	pthread_t	monitor_tid;

	if (!init_mtx(&monitor, forks, ruleset))
		return ;
	if (!launch_philos(ruleset, &monitor, philo, forks))
		return ;
	if (!thread_monitor(&monitor, &monitor_tid, philo))
		return ;
	i = -1;
	while (++i < ruleset->philo_nb)
		pthread_detach(philo[i].tid);
	destroy_mtx(ruleset, forks, philo, &monitor);
}
