/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:45:39 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/31 10:57:55 by halnuma          ###   ########.fr       */
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

void	ft_usleep(size_t sleep, t_philo *philo)
{
	size_t	timer;

	timer = 0;
	while (check_status(philo) && timer < sleep * 1000)
	{
		usleep(100);
		timer += 100;
	}
}
