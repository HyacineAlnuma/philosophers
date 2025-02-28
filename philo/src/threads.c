/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:40:08 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/28 12:07:53 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_if_alive(t_philo *philo)
{
	update_time(philo);
	pthread_mutex_lock(&philo->ruleset->rules_mutex);
	if ((philo->t_current - philo->t_last_meal) > philo->ruleset->t_die)
	{
		printf("%s[%ldms] - %d died%s\n", C_RED, philo->timestamp, philo->id, C_END);
		philo->is_alive = 0;
	}
	pthread_mutex_unlock(&philo->ruleset->rules_mutex);
}

int	try_to_eat(t_philo *philo)
{
	int	right_fork;
	int	left_fork;

	left_fork = philo->id - 1;
	right_fork = philo->id;
	pthread_mutex_lock(&philo->ruleset->rules_mutex);
	if (philo->id == philo->ruleset->philo_nb)
		right_fork = 0;
	pthread_mutex_unlock(&philo->ruleset->rules_mutex);
	pthread_mutex_lock(&philo->fork[left_fork].fork_mutex);
	// printf("PHILO:%d  as taken LEFT FORK:%d\n", philo->id, philo->fork[left_fork].id);
	check_if_alive(philo);
	if (!philo->is_alive)
	{
		pthread_mutex_unlock(&philo->fork[left_fork].fork_mutex);
		return (0);
	}
	pthread_mutex_lock(&philo->fork[right_fork].fork_mutex);
	// printf("PHILO:%d  as taken RIGHT FORK:%d\n", philo->id, philo->fork[right_fork].id);
	check_if_alive(philo);
	if (!philo->is_alive)
	{
		pthread_mutex_unlock(&philo->fork[left_fork].fork_mutex);
		pthread_mutex_unlock(&philo->fork[right_fork].fork_mutex);
		return (0);
	}
	p_eat(philo);
	pthread_mutex_unlock(&philo->fork[left_fork].fork_mutex);
	pthread_mutex_unlock(&philo->fork[right_fork].fork_mutex);
	return (1);
}

void	p_live(t_philo *philo)
{
	while (1)
	{
		if (!try_to_eat(philo))
			break ;
		p_sleep(philo);
		p_think(philo);
	}
}

void	*start_routine(void *data)
{
	t_philo			*philo;

	philo = (t_philo *)data;
	p_init(philo);
	p_live(philo);
	return (NULL);
}

void	create_threads(t_rules *ruleset)
{
	int			i;
	pthread_t	*tid;
	t_philo		*philo;
	t_fork		*fork;

	tid = (pthread_t *)malloc(sizeof(pthread_t) * (ruleset->philo_nb + 1));
	philo = (t_philo *)malloc(sizeof(t_philo) * (ruleset->philo_nb + 1));
	fork = (t_fork *)malloc(sizeof(t_fork) * (ruleset->philo_nb + 1));

	i = 0;
	pthread_mutex_init(&ruleset->rules_mutex, NULL);
	while (i < ruleset->philo_nb)
	{
		fork[i].id = i + 1;
		pthread_mutex_init(&fork[i].fork_mutex, NULL);
		philo[i].ruleset = ruleset;
		philo[i].id = i + 1;
		philo[i].fork = fork;
		pthread_create(&tid[i], NULL, start_routine, &philo[i]);
		i++;
	}
	while (--i >= 0)
	{
		pthread_join(tid[i], NULL);
		pthread_mutex_destroy(&fork[i].fork_mutex);
	}
	pthread_mutex_destroy(&ruleset->rules_mutex);
}
