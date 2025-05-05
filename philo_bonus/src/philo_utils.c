/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:57:16 by halnuma           #+#    #+#             */
/*   Updated: 2025/05/05 11:25:03 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*death_checker(void *data)
{
	t_philo	*philo;
	t_death	*death;
	int		i;
	size_t	current;

	death = (t_death *)data;
	philo = &death->monitor->philo[death->index];
	while (1)
	{
		sem_wait(philo->sems->s_death);
		current = get_current_time();
		i = 0;
		if ((current - philo->t_last_meal) >= philo->ruleset->t_die)
		{
			print_state(philo, "died", C_RED);
			while (i < philo->ruleset->philo_nb)
			{
				sem_post(philo->sems->s_meals);
				i++;
			}
			exit(EXIT_SUCCESS);
		}
		sem_post(philo->sems->s_death);
	}
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
