/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:30:26 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/19 18:29:56 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_rules
{
	int	philo_nb;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	meals_nb;
}	t_rules;

typedef struct s_philo
{
	pthread_mutex_t	id_mutex;
	int				id;
}	t_philo;

void	init_ruleset(t_rules *ruleset, char **av);
void	create_threads(t_rules *ruleset);

#endif