/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:38:06 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/31 10:27:18 by halnuma          ###   ########.fr       */
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

void	init_sems_bis(t_sem *sems)
{
	sems->s_write = sem_open("/write", O_CREAT | O_EXCL, 0666, 1);
	if (sems->s_write == SEM_FAILED)
	{
		perror("sem_open");
		sem_close(sems->s_forks);
		sem_unlink("/forks");
		exit(EXIT_FAILURE);
	}
	sems->s_meals = sem_open("/meals", O_CREAT | O_EXCL, 0666, 0);
	if (sems->s_meals == SEM_FAILED)
	{
		perror("sem_open");
		sem_close(sems->s_forks);
		sem_close(sems->s_write);
		sem_unlink("/write");
		sem_unlink("/forks");
		exit(EXIT_FAILURE);
	}
}

void	init_sems(t_sem *sems, t_rules *ruleset)
{
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/meals");
	sems->s_forks = sem_open(
			"/forks", O_CREAT | O_EXCL, 0666, ruleset->philo_nb
			);
	if (sems->s_forks == SEM_FAILED)
	{
		perror("sem_open");
		exit(EXIT_FAILURE);
	}
	init_sems_bis(sems);
}
