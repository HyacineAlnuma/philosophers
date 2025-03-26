/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:45:39 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/26 11:35:19 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	update_time(t_philo *philo)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
	{
		ft_putstr_fd("Error: gettimeofday failed.\n", 2);
		pthread_mutex_lock(philo->alive_mutex);
		*philo->alive = 0;
		pthread_mutex_unlock(philo->alive_mutex);
		return ;
	}
	pthread_mutex_lock(&(philo->time_mutex));
	philo->t_current = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	philo->ts = philo->t_current - philo->t_start;
	pthread_mutex_unlock(&(philo->time_mutex));
}
