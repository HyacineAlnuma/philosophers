/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:38:06 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/25 11:32:05 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	init_f_mutex(t_rules *ruleset, t_mutex *forks)
{
	int	i;

	i = 0;
	while (i < ruleset->philo_nb)
	{
		if (pthread_mutex_init(&forks[i], NULL))
		{
			ft_putstr_fd("Error: mutex init failed.\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_mtx(t_monitor *monitor, t_mutex *forks, t_rules *ruleset)
{
	if (pthread_mutex_init(&(monitor->alive_mutex), NULL))
	{
		ft_putstr_fd("Error: mutex init failed.\n", 2);
		return (0);
	}
	if (pthread_mutex_init(&(monitor->meals_mutex), NULL))
	{
		ft_putstr_fd("Error: mutex init failed.\n", 2);
		return (0);
	}
	if (pthread_mutex_init(&(monitor->print_mutex), NULL))
	{
		ft_putstr_fd("Error: mutex init failed.\n", 2);
		return (0);
	}
	if (!init_f_mutex(ruleset, forks))
		return (0);
	return (1);
}

int	p_init(t_philo *philo, int id, t_rules *ruleset)
{
	struct timeval	tv;

	philo->ruleset = ruleset;
	philo->id = id;
	philo->ut_sleep = philo->ruleset->t_sleep * 1000;
	philo->ut_eat = philo->ruleset->t_eat * 1000;
	if (gettimeofday(&tv, NULL))
	{
		ft_putstr_fd("Error: gettimeofday failed.\n", 2);
		return (0);
	}
	philo->t_start = ((&tv)->tv_sec * 1000) + ((&tv)->tv_usec / 1000);
	philo->t_last_meal = philo->t_start;
	philo->ts = 0;
	philo->meals_nb = 0;
	if (pthread_mutex_init(&philo->time_mutex, NULL))
	{
		ft_putstr_fd("Error: mutex init failed.\n", 2);
		return (0);
	}
	return (1);
}

void	p_init_bis(t_philo *philo, t_mutex *fork, t_monitor *monitor)
{
	philo->l_fork = fork;
	philo->alive = &monitor->alive;
	philo->alive_mutex = &monitor->alive_mutex;
	philo->meals_eaten = &monitor->meals_eaten;
	philo->meals_mutex = &monitor->meals_mutex;
	philo->print_mutex = &monitor->print_mutex;
}
