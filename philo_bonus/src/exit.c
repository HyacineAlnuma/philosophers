/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 10:27:53 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/02 12:31:47 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	kill_all_philos(t_monitor *monitor, int bool)
{
	int	i;

	i = -1;
	close_sems(monitor->sems);
	if (bool)
	{
		while (++i < monitor->ruleset->philo_nb)
		{
			pthread_mutex_lock(&monitor->m_pid);
			if (monitor->pids[i] != -1)
				kill(monitor->pids[i], SIGKILL);
			pthread_mutex_unlock(&monitor->m_pid);
		}
	}
	pthread_mutex_destroy(&monitor->m_pid);
}

void	close_sems(t_sem *sems)
{
	sem_close(sems->s_forks);
	sem_close(sems->s_write);
	sem_close(sems->s_meals);
	sem_close(sems->s_death);
	sem_unlink("/forks");
	sem_unlink("/meals");
	sem_unlink("/write");
	sem_unlink("/death");
}
