/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:28:45 by halnuma           #+#    #+#             */
/*   Updated: 2025/03/03 12:38:06 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_rules	*ruleset;

	ruleset = (t_rules *)malloc(sizeof(t_rules));
	if (!ruleset)
		return (1);
	if (ac == 5 || ac == 6)
	{
		if (ac == 6 && ft_atoi(av[5]) == 0)
		{
			free(ruleset);
			return (0);
		}
		init_ruleset(ruleset, av);
		create_threads(ruleset);
	}
	else
		printf("Error: Bad number of arguments.");
	free(ruleset);
	return (0);
}
