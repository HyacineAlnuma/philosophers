/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:54:02 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/15 10:37:04 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	lock_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->r_fork);
	else
		pthread_mutex_lock(philo->l_fork);
	print_state(philo, "has taken a fork", C_YEL);
	if (philo->ruleset->philo_nb == 1)
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_lock(philo->alive_mutex);
		*philo->alive = 0;
		pthread_mutex_unlock(philo->alive_mutex);
		usleep(philo->ruleset->t_die * 1000);
		print_state(philo, "died", C_RED);
		return ;
	}
	if (philo->id % 2 == 0)
		pthread_mutex_lock(philo->l_fork);
	else
		pthread_mutex_lock(philo->r_fork);
	print_state(philo, "has taken a fork", C_YEL);
}

void	p_eat(t_philo *philo)
{
	lock_forks(philo);
	pthread_mutex_lock(&(philo->time_mutex));
	philo->t_last_meal = get_current_time(philo);
	pthread_mutex_unlock(&(philo->time_mutex));
	print_state(philo, "is eating", C_GRN);
	pthread_mutex_lock(philo->meals_mutex);
	philo->meals_nb++;
	pthread_mutex_unlock(philo->meals_mutex);
	custom_usleep(philo->ruleset->t_eat, philo);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	p_sleep(t_philo *philo)
{
	print_state(philo, "is sleeping", C_CYN);
	custom_usleep(philo->ruleset->t_sleep, philo);
}

void	p_think(t_philo *philo)
{
	print_state(philo, "is thinking", C_MAG);
	usleep(500);
}
