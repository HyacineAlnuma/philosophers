/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:54:02 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/14 13:54:55 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* TODO : protect gettimeofday
*/

void	update_time(t_philo *philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&philo->time_mutex);
	philo->t_current = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	philo->ts = philo->t_current - philo->t_start;
	pthread_mutex_unlock(&philo->time_mutex);
	// pthread_mutex_lock(&philo->time_mutex);
	// philo->t_current = get_current_time();
	// philo->ts = philo->t_current - philo->t_start;
	// pthread_mutex_unlock(&philo->time_mutex);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
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
	philo->t_start = get_current_time();
	philo->t_last_meal = philo->t_start;
	philo->ts = 0;
	philo->meals_nb = 0;
	pthread_mutex_init(&philo->time_mutex, NULL);
}
