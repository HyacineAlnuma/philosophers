/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 09:30:32 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/07 14:34:24 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	kill_all_philos(t_philo *philo)
{
	int	i;

	i = -1;
	while (++i < philo->ruleset->philo_nb)
	{
		kill(philo->pid[i], SIGINT);
	}
}

void	check_if_alive(t_philo *philo)
{
	update_time(philo);
	if ((philo->t_current - philo->t_last_meal) >= philo->ruleset->t_die)
	{
		*philo->alive = 0;
		print_state(philo, "died", C_RED);
		// kill_all_philos(philo);
		return ;
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

void	process_philo(t_philo *philo, sem_t *forks)
{
	while (philo->alive)
	{
		p_eat(philo, forks);
		p_sleep(philo);
		p_think(philo);
	}
	// kill_all_philos(philo);
	// sem_close(forks);
	exit (EXIT_SUCCESS);
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

void	create_processes(t_rules *ruleset)
{
	t_monitor	monitor;
	pthread_t	monitor_tid;
	t_philo		philo[1024];
	// pid_t		pids[1024];
	sem_t		*forks;
	int			i;

	monitor.alive = 1;
	monitor.meals_eaten = 0;
	monitor.philo = philo;
	sem_unlink("/forks");
	forks = sem_open("/forks", O_CREAT | O_EXCL, 0666, ruleset->philo_nb);
	if (forks == SEM_FAILED)
	{
		perror("sem_open");
		exit(1);
	}
	// sem_wait(forks);
	// printf("yo\n");
	// sem_post(forks);
	// sem_init(&forks, 1, ruleset->philo_nb);
	i = -1;
	while (++i < ruleset->philo_nb)
	{
		philo[i].pid = monitor.pids;
		p_init(&philo[i], (i + 1), ruleset, &monitor);
		philo[i].pid[i] = fork();
		if (philo[i].pid[i] == -1)
			return ;
		else if (philo[i].pid[i] == 0)
		{
			process_philo(&philo[i], forks);
		}
	}
	pthread_create(&monitor_tid, NULL, monitor_routine, &monitor);
	pthread_join(monitor_tid, NULL);
	// (void)monitor_tid;
	i = -1;
	while (++i < ruleset->philo_nb)
		waitpid(philo[i].pid[i], NULL, 0);
	sem_close(forks);
	sem_unlink("/forks");
}

