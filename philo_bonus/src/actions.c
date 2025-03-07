/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:54:02 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/07 12:08:53 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	update_time(t_philo *philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	philo->t_current = ((&tv)->tv_sec * 1000) + ((&tv)->tv_usec / 1000);
	philo->ts = philo->t_current - philo->t_start;
}

void	p_eat(t_philo *philo, sem_t *forks)
{
	sem_wait(forks);
	print_state(philo, "has taken a fork", C_YEL);
	sem_wait(forks);
	print_state(philo, "has taken a fork", C_YEL);
	print_state(philo, "is eating", C_GRN);
	usleep(philo->ut_eat);
	sem_post(forks);
	sem_post(forks);
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

void	p_init(t_philo *philo, int id, t_rules *ruleset, t_monitor *monitor)
{
	struct timeval	tv;

	philo->ruleset = ruleset;
	philo->id = id;
	philo->ut_sleep = philo->ruleset->t_sleep * 1000;
	philo->ut_eat = philo->ruleset->t_eat * 1000;
	gettimeofday(&tv, NULL);
	philo->t_start = ((&tv)->tv_sec * 1000) + ((&tv)->tv_usec / 1000);
	philo->t_last_meal = philo->t_start;
	philo->ts = 0;
	philo->meals_nb = 0;
	philo->alive = &monitor->alive;
	philo->meals_eaten = &monitor->meals_eaten;
}
