/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:38:06 by halnuma           #+#    #+#             */
/*   Updated: 2025/05/05 11:15:50 by halnuma          ###   ########.fr       */
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

void	p_init(t_philo *philo, int id, t_rules *ruleset)
{
	philo->ruleset = ruleset;
	philo->t_current = 0;
	philo->id = id;
	philo->ut_sleep = philo->ruleset->t_sleep * 1000;
	philo->ut_eat = philo->ruleset->t_eat * 1000;
	philo->t_start = get_current_time();
	philo->t_last_meal = philo->t_start;
	philo->ts = 0;
	philo->meals_nb = 0;
}

int	init_monitor(t_monitor *m, t_philo *philo, t_rules *ruleset, t_sem *sems)
{
	int	i;

	i = 0;
	m->philo = philo;
	m->sems = sems;
	m->ruleset = ruleset;
	i = 0;
	while (i < PHILO_MAX)
	{
		m->pids[i] = -1;
		i++;
	}
	return (1);
}
