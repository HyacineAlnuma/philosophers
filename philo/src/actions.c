/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:54:02 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/28 15:22:08 by halnuma          ###   ########.fr       */
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

int	p_eat(t_philo *philo)
{
	update_time(philo);
	philo->t_last_meal = philo->t_current;
	printf("%s[%ldms] - %d is eating MEAL:%ld CUR:%ld START:%ld%s\n", C_GRN, philo->timestamp, philo->id, (philo->t_last_meal), philo->t_current, philo->t_start, C_END);
	usleep(philo->ut_eat);
	return (0);
}

int	p_sleep(t_philo *philo)
{
	update_time(philo);
	printf("%s[%ldms] - %d is sleeping%s\n", C_CYN, philo->timestamp, philo->id, C_END);
	usleep(philo->ut_sleep);
	return (0);
}

int	p_think(t_philo *philo)
{
	update_time(philo);
	printf("%s[%ldms] - %d is thinking%s\n", C_MAG, philo->timestamp, philo->id, C_END);
	return (0);
}

void	p_init(t_philo *philo)
{
	struct timeval	tv;

	philo->ut_sleep = philo->ruleset->t_sleep * 1000;
	philo->ut_eat = philo->ruleset->t_eat * 1000;
	gettimeofday(&tv, NULL);
	philo->t_start = ((&tv)->tv_sec * 1000) + ((&tv)->tv_usec / 1000);
	philo->t_last_meal = philo->t_start;
	philo->timestamp = 0;
	philo->is_alive = 1;
	printf("[%ldms] - %d is awake\n", philo->timestamp, philo->id);
}
