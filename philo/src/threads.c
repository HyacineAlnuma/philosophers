/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:07:44 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/19 18:34:34 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_routine(void *data)
{
	struct timeval	tv;
	int				philo_id;
	//t_philo			*philo;

	gettimeofday(&tv, NULL);
	philo_id = *(int *)data;
	printf("[%d] - %d is alive\n", 0, philo_id);
	sleep(1);
	printf("[%ld] - %d is awake\n", tv.tv_usec, philo_id);
	return (NULL);
}

void	create_threads(t_rules *ruleset)
{
	int			i;
	pthread_t	tid[1024];
	int			philo_id[1024];

	i = -1;
	//philo.id = 0;
	//pthread_mutex_init(&philo.id_mutex, NULL);
	while (++i < ruleset->philo_nb)
	{
		philo_id[i] = i + 1;
		pthread_create(&tid[i], NULL, start_routine, &philo_id[i]);
	}
	i = -1;
	while (++i < ruleset->philo_nb)
		pthread_join(tid[i], NULL);
}
