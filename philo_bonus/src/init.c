/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:38:06 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/25 11:18:43 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	init_ruleset(t_rules *ruleset, char **av)
{
	ruleset->philo_nb = ft_atoi(av[1]);
	ruleset->t_die = ft_atoi(av[2]);
	ruleset->t_eat = ft_atoi(av[3]);
	ruleset->t_sleep = ft_atoi(av[4]);
	if (av[5])
		ruleset->meals_nb = ft_atoi(av[5]);
	else
		ruleset->meals_nb = 0;
}

void	p_init(t_philo *philo, int id, t_rules *ruleset, t_monitor *monitor)
{
	struct timeval	tv;

	philo->ruleset = ruleset;
	philo->t_current = 0;
	philo->t_last_meal = 0;
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
