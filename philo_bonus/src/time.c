/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:45:39 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/02 10:36:08 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	update_time(t_philo *philo)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
	{
		ft_putstr_fd("Error: gettimeofday failed.\n", 2);
		return ;
	}
	philo->t_current = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	philo->ts = philo->t_current - philo->t_start;
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL))
	{
		ft_putstr_fd("Error: gettimeofday failed.\n", 2);
		return (0);
	}
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
