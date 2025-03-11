/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:40:08 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/11 10:22:59 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	int	i;

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
		if (!monitor->alive || monitor->meals_eaten)
			break ;
		i++;
	}
	return (NULL);
}

/* TODO : protect thread creation and join?
*/

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
