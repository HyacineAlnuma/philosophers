/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 09:30:32 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/31 15:44:19 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	process_philo(t_monitor *monitor, int i)
{
	pthread_t	monitor_tid;
	t_philo		*philo;
	t_death		death;

	philo = &monitor->philo[i];
	death.monitor = monitor;
	death.index = i;
	if (pthread_create(&monitor_tid, NULL, death_checker, &death))
	{
		ft_putstr_fd("Error: thread creation failed.\n", 2);
		kill_all_philos(monitor, 0);
		exit(EXIT_FAILURE);
	}
	pthread_detach(monitor_tid);
	while (1)
	{
		p_eat(philo);
		p_sleep(philo);
		p_think(philo);
	}
}

void	*meals_monitor(void *data)
{
	t_monitor	*monitor;
	int			finished_eating;

	monitor = (t_monitor *)data;
	finished_eating = 0;
	while (1)
	{
		sem_wait(monitor->sems->s_meals);
		finished_eating++;
		if (finished_eating == monitor->ruleset->philo_nb)
		{
			kill_all_philos(monitor, 2);
			return (NULL);
		}
	}
	return (NULL);
}

void	launch_philos(t_philo *philo, t_rules *r, t_monitor *m, t_sem *sems)
{
	int	i;

	i = -1;
	while (++i < r->philo_nb)
	{
		philo[i].pid = m->pids;
		philo[i].sems = sems;
		p_init(&philo[i], (i + 1), r, m);
		philo[i].pid[i] = fork();
		if (philo[i].pid[i] == -1)
			return ;
		else if (philo[i].pid[i] == 0)
			process_philo(m, i);
	}
}


void	create_processes(t_rules *ruleset)
{
	t_monitor	monitor;
	pthread_t	monitor_tid;
	t_philo		philo[PHILO_MAX];
	t_sem		sems;
	int			i;

	monitor.alive = 1;
	monitor.meals_eaten = 0;
	monitor.philo = philo;
	monitor.sems = &sems;
	monitor.ruleset = ruleset;
	i = 0;
	while (i < PHILO_MAX)
	{
		monitor.pids[i] = -1;
		i++;
	}
	init_sems(&sems, ruleset);
	if (pthread_create(&monitor_tid, NULL, meals_monitor, &monitor))
	{
		ft_putstr_fd("Error: thread creation failed.\n", 2);
		exit(EXIT_FAILURE);
	}
	pthread_detach(monitor_tid);
	launch_philos(philo, ruleset, &monitor, &sems);
	waitpid(-1, NULL, 0);
	close_sems(&sems);
}
