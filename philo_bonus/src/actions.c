/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:54:02 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/31 13:35:12 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	p_eat(t_philo *philo)
{
	sem_wait(philo->sems->s_forks);
	print_state(philo, "has taken a fork", C_YEL);
	sem_wait(philo->sems->s_forks);
	print_state(philo, "has taken a fork", C_YEL);
	print_state(philo, "is eating", C_GRN);
	// sem_wait(philo->sems->s_death);
	philo->t_last_meal = philo->t_current;
	philo->meals_nb++;
	// sem_post(philo->sems->s_death);
	if (philo->meals_nb == philo->ruleset->meals_nb)
		sem_post(philo->sems->s_meals);
	usleep(philo->ut_eat);
	sem_post(philo->sems->s_forks);
	sem_post(philo->sems->s_forks);
}

void	p_sleep(t_philo *philo)
{
	print_state(philo, "is sleeping", C_CYN);
	usleep(philo->ut_sleep);
}

void	p_think(t_philo *philo)
{
	print_state(philo, "is thinking", C_MAG);
}
