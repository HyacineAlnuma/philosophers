/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:27:53 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/31 10:46:27 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	kill_all_philos(t_philo *philo)
{
	int	i;

	i = -1;
	while (++i < philo->ruleset->philo_nb)
		kill(philo->pid[i], SIGINT);
}

void	close_sems(t_sem *sems)
{
	sem_close(sems->s_forks);
	sem_close(sems->s_write);
	sem_close(sems->s_meals);
	sem_unlink("/forks");
	sem_unlink("/meals");
	sem_unlink("/write");
}
