/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 09:30:32 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/31 10:29:36 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	process_philo(t_philo *philo)
{
	pthread_t	monitor_tid;

	if (pthread_create(&monitor_tid, NULL, death_checker, philo))
	{
		ft_putstr_fd("Error: thread creation failed.\n", 2);
		exit(EXIT_FAILURE);
	}
	pthread_detach(monitor_tid);
	while (check_status(philo))
	{
		p_eat(philo);
		p_sleep(philo);
		p_think(philo);
	}
	exit(EXIT_SUCCESS);
}

void	*meals_monitor(void *data)
{
	t_philo	*philo;
	int		finished_eating;

	philo = (t_philo *)data;
	finished_eating = 0;
	while (1)
	{
		sem_wait(philo->sems->s_meals);
		finished_eating++;
		if (finished_eating == philo->ruleset->philo_nb)
			kill_all_philos(philo);
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
		{
			process_philo(&philo[i]);
		}
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
	init_sems(&sems, ruleset);
	launch_philos(philo, ruleset, &monitor, &sems);
	if (pthread_create(&monitor_tid, NULL, meals_monitor, &philo[0]))
	{
		ft_putstr_fd("Error: thread creation failed.\n", 2);
		exit(EXIT_FAILURE);
	}
	pthread_detach(monitor_tid);
	i = -1;
	while (++i < ruleset->philo_nb)
		waitpid(philo[i].pid[i], NULL, 0);
	close_sems(&sems);
}
