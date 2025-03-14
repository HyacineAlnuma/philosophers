/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 09:30:32 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/14 10:14:59 by halnuma          ###   ########.fr       */
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

void	check_if_alive(t_philo *philo)
{
	update_time(philo);
	if ((philo->t_current - philo->t_last_meal) >= philo->ruleset->t_die)
	{
		*philo->alive = 0;
		// printf("yo %ld\n", (philo->t_current - philo->t_last_meal));
		print_state(philo, "died", C_RED);
		kill_all_philos(philo);
		exit(EXIT_SUCCESS);
	}
}

void	check_if_all_meals_eaten(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo[0].ruleset->philo_nb)
	{
		if (philo[i].meals_nb < philo[0].ruleset->meals_nb)
			return ;
		i++;
	}
	*philo->meals_eaten = 1;
	return ;
}

int	check_status(t_philo *philo)
{
	//check_if_alive(philo);
	// check_if_all_meals_eaten(philo);
	if (!(*philo->alive) || *philo->meals_eaten)
	{
		return (0);
	}
	return (1);
}

void	*death_checker(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (1)
		check_if_alive(philo);
	return (NULL);
}

void	process_philo(t_philo *philo)
{
	pthread_t	monitor_tid;

	pthread_create(&monitor_tid, NULL, death_checker, philo);
	pthread_detach(monitor_tid);
	while (check_status(philo))
	{
		p_eat(philo);
		p_sleep(philo);
		p_think(philo);
	}
	exit(EXIT_SUCCESS);
}

void	*monitor_routine(void *data)
{
	t_monitor	*monitor;
	int			i;

	monitor = (t_monitor *)data;
	i = 0;
	while (1)
	{
		if (i == monitor->philo[0].ruleset->philo_nb)
			i = 0;
		check_if_alive(&monitor->philo[i]);
		if (monitor->philo[0].ruleset->meals_nb)
			check_if_all_meals_eaten(monitor->philo);
		if (!monitor->alive || monitor->meals_eaten)
		{
			// printf("yo\n");
			//kill(0, SIGINT);
			kill_all_philos(monitor->philo);
			break ;
		}
		i++;
	}
	return (NULL);
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
	t_philo		philo[1024];
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
	pthread_create(&monitor_tid, NULL, meals_monitor, &philo[0]);
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
