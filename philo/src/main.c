/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: halnuma <halnuma@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 16:28:45 by halnuma           #+#    #+#             */
/*   Updated: 2025/04/02 11:39:02 by halnuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_args(int ac, char **av, t_rules *ruleset)
{
	int	i;
	int	j;

	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (!ft_isdigit(av[i][j]))
				return (0);
		}
		if (j > 10)
			return (0);
		if (ft_atoi(av[i]) == -1)
			return (0);
	}
	if (ft_atoi(av[1]) > PHILO_MAX)
		return (0);
	if ((ac == 6 && ft_atoi(av[5]) == 0)
		|| ft_atoi(av[1]) == 0)
	{
		free(ruleset);
		return (2);
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_rules	*ruleset;

	ruleset = (t_rules *)malloc(sizeof(t_rules));
	if (!ruleset)
		return (1);
	if (ac == 5 || ac == 6)
	{
		if (!check_args(ac, av, ruleset))
		{
			ft_putstr_fd("Error: Bad arguments.\n", 2);
			free(ruleset);
			return (1);
		}
		if (check_args(ac, av, ruleset) == 2)
			return (0);
		init_ruleset(ruleset, av);
		create_threads(ruleset);
	}
	else
		ft_putstr_fd("Error: Bad number of arguments.\n", 2);
	free(ruleset);
	return (0);
}
