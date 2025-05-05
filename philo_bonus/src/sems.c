/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sems.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:15:29 by halnuma           #+#    #+#             */
/*   Updated: 2025/05/05 14:40:16 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_sems_ter(t_sem *sems)
{
	sems->s_pid = sem_open("/pid", O_CREAT | O_EXCL, 0666, 1);
	if (sems->s_pid == SEM_FAILED)
	{
		ft_putstr_fd("sem_open error", 2);
		sem_close(sems->s_forks);
		sem_close(sems->s_write);
		sem_close(sems->s_meals);
		sem_close(sems->s_death);
		sem_unlink("/write");
		sem_unlink("/forks");
		sem_unlink("/meals");
		sem_unlink("/death");
		return (0);
	}
	return (1);
}

int	init_sems_bis(t_sem *sems)
{
	sems->s_meals = sem_open("/meals", O_CREAT | O_EXCL, 0666, 0);
	if (sems->s_meals == SEM_FAILED)
	{
		ft_putstr_fd("sem_open error", 2);
		sem_close(sems->s_forks);
		sem_close(sems->s_write);
		sem_unlink("/write");
		sem_unlink("/forks");
		return (0);
	}
	sems->s_death = sem_open("/death", O_CREAT | O_EXCL, 0666, 1);
	if (sems->s_death == SEM_FAILED)
	{
		ft_putstr_fd("sem_open error", 2);
		sem_close(sems->s_forks);
		sem_close(sems->s_write);
		sem_close(sems->s_meals);
		sem_unlink("/write");
		sem_unlink("/forks");
		sem_unlink("/meals");
		return (0);
	}
	return (init_sems_ter(sems));
}

int	init_sems(t_sem *sems, t_rules *ruleset)
{
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/meals");
	sem_unlink("/death");
	sem_unlink("/pid");
	sems->s_forks = sem_open(
			"/forks", O_CREAT | O_EXCL, 0666, ruleset->philo_nb
			);
	if (sems->s_forks == SEM_FAILED)
	{
		ft_putstr_fd("sem_open error", 2);
		return (0);
	}
	sems->s_write = sem_open("/write", O_CREAT | O_EXCL, 0666, 1);
	if (sems->s_write == SEM_FAILED)
	{
		ft_putstr_fd("sem_open error", 2);
		sem_close(sems->s_forks);
		sem_unlink("/forks");
		return (0);
	}
	return (init_sems_bis(sems));
}
