/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:57:16 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/25 13:02:03 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_philo *philo, char *action, char *color)
{
	update_time(philo);
	if (!check_status(philo) && ft_strcmp(action, "died"))
		return ;
	pthread_mutex_lock(&philo->time_mutex);
	pthread_mutex_lock(philo->print_mutex);
	printf("[%ldms] - %s%d %s%s\n", philo->ts, color, philo->id, action, C_END);
	pthread_mutex_unlock(&philo->time_mutex);
	pthread_mutex_unlock(philo->print_mutex);
}

void	destroy_mtx(t_rules *ruleset, t_mutex *forks, t_philo *p, t_monitor *m)
{
	int	i;

	i = 0;
	while (i < ruleset->philo_nb)
	{
		pthread_mutex_destroy(&forks[i]);
		pthread_mutex_destroy(&p[i].time_mutex);
		i++;
	}
	pthread_mutex_destroy(&(m->alive_mutex));
	pthread_mutex_destroy(&(m->meals_mutex));
	pthread_mutex_destroy(&(m->print_mutex));
}

int	check_status(t_philo *philo)
{
	pthread_mutex_lock(philo->alive_mutex);
	pthread_mutex_lock(philo->meals_mutex);
	if (!(*philo->alive) || *philo->meals_eaten)
	{
		pthread_mutex_unlock(philo->meals_mutex);
		pthread_mutex_unlock(philo->alive_mutex);
		return (0);
	}
	pthread_mutex_unlock(philo->alive_mutex);
	pthread_mutex_unlock(philo->meals_mutex);
	return (1);
}

void	check_if_alive(t_philo *philo)
{
	update_time(philo);
	pthread_mutex_lock(&philo->time_mutex);
	if ((philo->t_current - philo->t_last_meal) >= philo->ruleset->t_die)
	{
		pthread_mutex_unlock(&philo->time_mutex);
		pthread_mutex_lock(philo->alive_mutex);
		*philo->alive = 0;
		pthread_mutex_unlock(philo->alive_mutex);
		print_state(philo, "died", C_RED);
		return ;
	}
	pthread_mutex_unlock(&philo->time_mutex);
}

void	check_if_all_meals_eaten(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo[0].ruleset->philo_nb)
	{
		pthread_mutex_lock(philo->meals_mutex);
		if (philo[i].meals_nb < philo[0].ruleset->meals_nb)
		{
			pthread_mutex_unlock(philo->meals_mutex);
			return ;
		}
		pthread_mutex_unlock(philo->meals_mutex);
		i++;
	}
	pthread_mutex_lock(philo->meals_mutex);
	*philo->meals_eaten = 1;
	pthread_mutex_unlock(philo->meals_mutex);
	return ;
}
