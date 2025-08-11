/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eulee <eulee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:26:27 by eulee             #+#    #+#             */
/*   Updated: 2025/08/11 16:39:29 by eulee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_rules		rules;
	t_philo		*philo;
	int			i;

	if (init_rules(&rules, ac, av) != 0)
	{
		write(2, "Error: wrong number of arguments or wrong arguments\n", 52);
		return (0);
	}
	rules.start_time = get_time_in_ms();
	if (init_philo(&rules, &philo) != 0)
		return (0);

	if (rules.nb_philo == 1)
	{
		handle_one_philo(philo);
		free_all(philo, &rules);
		return (0);
	}
	i = 0;
	while (i < rules.nb_philo)
	{
		if(pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]) != 0)
		{	
			write(2, "thread create error\n", 20);
			free_all(philo, &rules);
			return (0);
		}
		i++;
	}
	while (1 < rules.nb_philo)
	{
		if (monitoring(philo))
			break;
	}
	i = 0;
	while (i < rules.nb_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
	free_all(philo, &rules);
	return (0);
}
