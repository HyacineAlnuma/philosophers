/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:54:02 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/03 13:09:21 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	update_time(t_philo *philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	philo->t_current = ((&tv)->tv_sec * 1000) + ((&tv)->tv_usec / 1000);
	philo->timestamp = philo->t_current - philo->t_start;
}

void	p_eat(t_philo *philo)
{
	if (!check_status(philo))
		return ;
	pthread_mutex_lock(philo->l_fork);
	update_time(philo);
	if (!check_status(philo))
		return ;
	printf("%s[%ldms] - %d has taken a fork%s\n", C_YEL, philo->timestamp, philo->id, C_END);
	if (philo->ruleset->philo_nb == 1)
	{
		pthread_mutex_unlock(philo->l_fork);
		usleep(philo->ruleset->t_die *1000);
		return ;
	}
	pthread_mutex_lock(philo->r_fork);
	update_time(philo);
	if (!check_status(philo))
		return ;
	printf("%s[%ldms] - %d has taken a fork%s\n", C_YEL, philo->timestamp, philo->id, C_END);
	update_time(philo);
	philo->t_last_meal = philo->t_current;
	if (!check_status(philo))
		return ;
	printf("%s[%ldms] - %d is eating %s\n", C_GRN, philo->timestamp, philo->id, C_END);
	philo->meals_nb++;
	usleep(philo->ut_eat);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

void	p_sleep(t_philo *philo)
{
	update_time(philo);
	if (!check_status(philo))
		return ;
	printf("%s[%ldms] - %d is sleeping%s\n", C_CYN, philo->timestamp, philo->id, C_END);
	usleep(philo->ut_sleep);
}

void	p_think(t_philo *philo)
{
	update_time(philo);
	if (!check_status(philo))
		return ;
	printf("%s[%ldms] - %d is thinking%s\n", C_MAG, philo->timestamp, philo->id, C_END);
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
	philo->ut_sleep = philo->ruleset->t_sleep * 1000;
	philo->ut_eat = philo->ruleset->t_eat * 1000;
	gettimeofday(&tv, NULL);
	philo->t_start = ((&tv)->tv_sec * 1000) + ((&tv)->tv_usec / 1000);
	philo->t_last_meal = philo->t_start;
	philo->timestamp = 0;
	philo->meals_nb = 0;
}
