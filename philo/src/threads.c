/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 11:40:08 by halnuma           #+#    #+#             */
/*   Updated: 2025/02/28 15:45:21 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	MARCHE MAIS MEURS PENDANT LE SLEEP -> LE CHECK EST FAIT APRES LE SLEEP AU DEBUT DU EAT DONC MARCHE PAS
	-> FAIRE UN THREAD MONITOR QUI VA CHECK EN BOUCLE L'ETAT DE CHACUN DES PHILOS ET S'ILS ONT MANGE TOUS LEURS REPAS
	-> FAIRE UNE STRUCT GLOBALE QUI CONTIENT TOUS LES PHILOS ET PASSER CELLE CI A TOUS LES THREADS
	DANS LA STRUCT GLOBALE TU METS UN MUTEX ALIVE QUE TU MODIFIES DES QU'UN PHILO MEURT.
	DANS LES THREADS TU BOUCLES SUR LES ACTION TANT QUE CTTE GLOABLE ALIVE EST VRAIE
*/

void	check_if_alive(t_philo *philo)
{
	update_time(philo);
	// pthread_mutex_lock(&philo->ruleset->rules_mutex);
	if ((philo->t_current - philo->t_last_meal) > philo->ruleset->t_die)
	{
		printf("%sTS:%ld DIE:%ld MEAL:%ld CUR:%ld START:%ld%s\n", C_BLU, philo->timestamp, philo->ruleset->t_die, (philo->t_last_meal), philo->t_current, philo->t_start, C_END);
		printf("%s[%ldms] - %d died TIME:%ld %s\n", C_RED, philo->timestamp, philo->id, (philo->t_last_meal), C_END);
		philo->is_alive = 0;
	}
	// pthread_mutex_unlock(&philo->ruleset->rules_mutex);
}

int	try_to_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	update_time(philo);
	printf("%s[%ldms] - %d has taken a fork%s\n", C_YEL, philo->timestamp, philo->id, C_END);
	// // printf("PHILO:%d  as taken LEFT FORK:%d\n", philo->id, philo->fork[left_fork].id);
	check_if_alive(philo);
	if (!philo->is_alive)
	{
		printf("yo\n");
		pthread_mutex_unlock(philo->l_fork);
		return (0);
	}
	pthread_mutex_lock(philo->r_fork);
	update_time(philo);
	printf("%s[%ldms] - %d has taken a fork%s\n", C_YEL, philo->timestamp, philo->id, C_END);
	// // printf("PHILO:%d  as taken RIGHT FORK:%d\n", philo->id, philo->fork[right_fork].id);
	check_if_alive(philo);
	if (!philo->is_alive)
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		return (0);
	}
	p_eat(philo);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
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
	if (philo->id % 2 == 0)
		usleep(5);
	p_init(philo);
	p_live(philo);
	return (NULL);
}

void	create_threads(t_rules *ruleset)
{
	int			i;
	t_philo		philo[1024];
	pthread_mutex_t		forks[1024];

	i = 0;
	// pthread_mutex_init(&ruleset->rules_mutex, NULL);
	while (i < ruleset->philo_nb)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}

	i = 0;
	// pthread_mutex_init(&ruleset->rules_mutex, NULL);
	while (i < ruleset->philo_nb)
	{
		philo[i].ruleset = ruleset;
		philo[i].id = i + 1;
		philo[i].l_fork = &forks[i];
		if (i == ruleset->philo_nb - 1)
			philo[i].r_fork = &forks[0];
		else
			philo[i].r_fork = &forks[i + 1];
		pthread_create(&philo[i].tid, NULL, start_routine, &philo[i]);
		i++;
	}
	while (--i >= 0)
	{
		pthread_join(philo[i].tid, NULL);
	}
	while (i < ruleset->philo_nb)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	// pthread_mutex_destroy(&ruleset->rules_mutex);
}
