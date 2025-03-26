/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:54:02 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/26 11:36:46 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	pthread_mutex_lock(philo->meals_mutex);
	philo->meals_nb++;
	pthread_mutex_unlock(philo->meals_mutex);
	pthread_mutex_lock(&(philo->time_mutex));
	philo->t_last_meal = philo->t_current;
	usleep(philo->ut_eat);
	pthread_mutex_unlock(&(philo->time_mutex));
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
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
