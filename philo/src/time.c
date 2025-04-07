/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:45:39 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/07 09:38:09 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_current_time(t_philo *philo)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL))
	{
		ft_putstr_fd("Error: gettimeofday failed.\n", 2);
		pthread_mutex_lock(philo->alive_mutex);
		*philo->alive = 0;
		pthread_mutex_unlock(philo->alive_mutex);
		return (0);
	}
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	custom_usleep(size_t sleep, t_philo *philo)
{
	size_t	start_time;
	size_t	current_time;

	start_time = get_current_time(philo);
	while (1)
	{
		current_time = get_current_time(philo);
		if ((current_time - start_time) >= sleep)
			break ;
		if (!check_status(philo))
			break ;
		usleep(50);
	}
}
