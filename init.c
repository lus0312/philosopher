/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eulee <eulee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:51:10 by eulee             #+#    #+#             */
/*   Updated: 2025/08/09 16:37:21 by eulee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_rules(t_rules *rules, int ac, char **av)
{
	int	i;

	i = 0;
	if (ac != 5 && ac != 6)
		return (1);
	if (ft_atoi(av[1]) <= 0)
		return (1);
	rules->nb_philo = ft_atoi(av[1]);
	rules->time_to_die = ft_atoi(av[2]);
	rules->time_to_eat = ft_atoi(av[3]);
	rules->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		rules->must_eat= ft_atoi(av[5]);
	else
		rules->must_eat = -1; // must_eat 매개변수 없는 경우 -1로 초기화
	rules->is_dead = 0;
	rules->start_time = 0;
	if (pthread_mutex_init(&rules->dead_mutex, NULL) != 0) //mutex init 성공시 0 리턴
		return (1);
	if (pthread_mutex_init(&rules->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&rules->dead_mutex);
		return (1);
	}
	rules->forks = malloc(sizeof(pthread_mutex_t) * (rules->nb_philo));
	if (!rules->forks)
	{
		free_rules(rules);
		return (1);
	}
	while (i < rules->nb_philo)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL) != 0)
		{
			free_rules(rules);
			free_forks(rules, i);
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_philo(t_rules *rules, t_philo **philos)
{
	int	i;
	int	j;

	*philos = malloc(sizeof(t_philo) * (rules->nb_philo)); 
	if (!*philos)
	{
		i = rules->nb_philo;
		free_forks(rules, i);
		free_rules(rules);
		return (1);
	}
	i = 0;
	while (i < rules->nb_philo)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].eat_count = 0;
		(*philos)[i].eat_flag = 0;
		(*philos)[i].last_eat = rules->start_time;
		(*philos)[i].rules = rules;
		(*philos)[i].left_fork = &rules->forks[i];
		if (rules->nb_philo == 1)	//철학자 1명인 경우 포크가 하나
			(*philos)[i].right_fork = NULL;
		else
			(*philos)[i].right_fork = &rules->forks[(i + 1) % rules->nb_philo];
		if (pthread_mutex_init(&(*philos)[i].meal_mutex, NULL) != 0)
		{
			j = rules->nb_philo;
			free_forks(rules, j);
			free_rules(rules);
			free_philos(philos, i);
			return (1);
		}
		i++;
	}
	return (0);
}
