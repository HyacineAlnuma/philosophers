/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:57:16 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/31 10:01:18 by halnuma          ###   ########.fr       */
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
	size_t	current;

	current = get_current_time();
	// printf("ID:%d CUR:%lu LM:%lu\n", philo->id, current, philo->t_last_meal);
	// update_time(philo);
	if ((current - philo->t_last_meal) >= philo->ruleset->t_die)
	{
		// printf("id:%d cur:%lu die:%zu\n", philo->id, (current - philo->t_last_meal), philo->ruleset->t_die);
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

void	print_state(t_philo *philo, char *action, char *color)
{
	size_t	current;
	size_t	ts;

	current = get_current_time();
	ts = current - philo->t_start;
	sem_wait(philo->sems->s_write);
	printf("[%ldms] - %s%d %s%s\n", ts, color, philo->id, action, C_END);
	if (!ft_strcmp(action, "died"))
		return ;
	sem_post(philo->sems->s_write);
}
