/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 09:30:32 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/31 10:00:50 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	process_philo(t_philo *philo)
{
	pthread_t	monitor_tid;

	// philo->t_start = get_current_time();
	// philo->t_last_meal = philo->t_start;
	if (pthread_create(&monitor_tid, NULL, death_checker, philo))
	{
		ft_putstr_fd("Error: thread creation failed.\n", 2);
		exit(EXIT_FAILURE);
	}
	// pthread_detach(monitor_tid);
	while (check_status(philo))
	{
		p_eat(philo);
		p_sleep(philo);
		p_think(philo);
	}
	pthread_detach(monitor_tid);
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
	sem_unlink("/forks");
	sem_unlink("/death");
	sem_unlink("/meals");
	sems.s_forks = sem_open("/forks", O_CREAT | O_EXCL, 0666, ruleset->philo_nb);
	if (sems.s_forks == SEM_FAILED)
	{
		perror("sem_open");
		exit(1);
	}
	sems.s_write = sem_open("/death", O_CREAT | O_EXCL, 0666, 1);
	if (sems.s_write == SEM_FAILED)
	{
		perror("sem_open");
		exit(1);
	}
	sems.s_meals = sem_open("/meals", O_CREAT | O_EXCL, 0666, 0);
	if (sems.s_meals == SEM_FAILED)
	{
		perror("sem_open");
		exit(1);
	}
	i = -1;
	while (++i < ruleset->philo_nb)
	{
		philo[i].pid = monitor.pids;
		philo[i].sems = &sems;
		p_init(&philo[i], (i + 1), ruleset, &monitor);
		philo[i].pid[i] = fork();
		if (philo[i].pid[i] == -1)
			return ;
		else if (philo[i].pid[i] == 0)
		{
			process_philo(&philo[i]);
		}
	}
	if (pthread_create(&monitor_tid, NULL, meals_monitor, &philo[0]))
	{
		ft_putstr_fd("Error: thread creation failed.\n", 2);
		exit(EXIT_FAILURE);
	}
	pthread_detach(monitor_tid);
	//(void)monitor_tid;
	i = -1;
	while (++i < ruleset->philo_nb)
		waitpid(philo[i].pid[i], NULL, 0);
	sem_close(sems.s_forks);
	sem_close(sems.s_write);
	sem_close(sems.s_meals);
	sem_unlink("/forks");
	sem_unlink("/meals");
	sem_unlink("/death");
}
