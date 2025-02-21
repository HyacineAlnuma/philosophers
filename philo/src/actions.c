/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:54:02 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/21 13:12:23 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	update_time(struct timeval *tv, t_philo *philo)
{
	gettimeofday(tv, NULL);
	philo->t_current = (tv->tv_sec * 1000) + (tv->tv_usec / 1000);
	philo->timestamp = philo->t_current - philo->t_start;
}

int	p_eat(struct timeval *tv, t_philo *philo)
{
	update_time(tv, philo);
	printf("[%ldms] - %d is eating\n", philo->timestamp, philo->id);
	usleep(philo->ut_eat);
	update_time(tv, philo);
	philo->t_last_meal = philo->t_current;
	return (0);
}

int	p_sleep(struct timeval *tv, t_philo *philo)
{
	update_time(tv, philo);
	printf("[%ldms] - %d is sleeping\n", philo->timestamp, philo->id);
	usleep(philo->ut_sleep);
	return (0);
}

int	p_think(struct timeval *tv, t_philo *philo)
{
	update_time(tv, philo);
	printf("[%ldms] - %d is thinking\n", philo->timestamp, philo->id);
	return (0);
}

void	p_init(struct timeval *tv, t_philo *philo)
{
	philo->is_alive = 1;
	philo->ut_sleep = philo->ruleset->t_sleep * 1000;
	philo->ut_eat = philo->ruleset->t_eat * 1000;
	gettimeofday(tv, NULL);
	philo->t_start = (tv->tv_sec * 1000) + (tv->tv_usec / 1000);
	philo->t_last_meal = philo->t_start;
	philo->timestamp = 0;
	printf("[%ldms] - %d is awake\n", philo->timestamp, philo->id);
}
