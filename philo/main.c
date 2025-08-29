/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eulee <eulee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:26:27 by eulee             #+#    #+#             */
/*   Updated: 2025/08/29 20:03:43 by eulee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork", 0);
	pthread_mutex_unlock(philo->left_fork);
	precise_usleep(philo->rules->time_to_die, philo);
	print_status(philo, "died", 1);
}

int	start_simulation(t_rules *rules, t_philo *philo)
{
	int	i;

	if (rules->nb_philo == 1)
	{
		handle_one_philo(philo);
		return (0);
	}
	i = 0;
	while (i < rules->nb_philo)
	{
		if (pthread_create(&philo[i].thread, NULL,
				philo_routine, &philo[i]) != 0)
		{
			write(2, "thread create error\n", 20);
			return (1);
		}
		i++;
	}
	while (1)
	{
		if (monitoring(philo, rules))
			break ;
	}
	return (0);
}

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
	if (init_mutexes(&rules) != 0)
		return (0);
	rules.start_time = get_time_in_ms();
	if (init_philo(&rules, &philo) != 0
		|| init_philo_mutex(&rules, philo) != 0)
		return (0);
	if (start_simulation(&rules, philo) != 0)
	{
		free_all(philo, &rules);
		return (0);
	}
	i = -1;
	while (++i < rules.nb_philo)
		pthread_join(philo[i].thread, NULL);
	free_all(philo, &rules);
	return (0);
}
