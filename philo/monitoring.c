/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eulee <eulee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:57:17 by eulee             #+#    #+#             */
/*   Updated: 2025/08/29 20:10:30 by eulee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death_or_full(t_philo *philos, int i)
{
	long long	now;

	now = get_time_in_ms();
	pthread_mutex_lock(&philos[i].meal_mutex);
	if (now - philos[i].last_eat >= philos[i].rules->time_to_die)
	{
		pthread_mutex_unlock(&philos[i].meal_mutex);
		pthread_mutex_lock(&philos[i].rules->dead_mutex);
		philos[i].rules->is_dead = 1;
		pthread_mutex_unlock(&philos[i].rules->dead_mutex);
		print_status(&philos[i], "died", 1);
		return (1);
	}
	if (philos[i].rules->must_eat != -1
		&& philos[i].eat_count == philos[i].rules->must_eat)
	{
		if (philos[i].eat_flag != 1)
		{
			pthread_mutex_lock(&philos[i].rules->must_eat_count_mutex);
			philos[i].rules->must_eat_count++;
			printf("must_eat_count[%d] : %d\n", i, philos[i].rules->must_eat_count);
			pthread_mutex_unlock(&philos[i].rules->must_eat_count_mutex);
			philos[i].eat_flag = 1;
		}
	}
	pthread_mutex_unlock(&philos[i].meal_mutex);
	return (0);
}

int	monitoring(void *arg, t_rules *rules)
{
	t_philo		*philos;
	int			i;

	philos = (t_philo *)arg;
	while (1)
	{
		i = 0;
		while (i < rules->nb_philo)
		{
			pthread_mutex_lock(&philos->rules->must_eat_count_mutex);
			if (philos->rules->must_eat_count == philos->rules->nb_philo)
			{
				pthread_mutex_unlock(&philos->rules->must_eat_count_mutex);
				pthread_mutex_lock(&philos[i].rules->dead_mutex);
				philos[i].rules->is_dead = 1;
				pthread_mutex_unlock(&philos[i].rules->dead_mutex);
				return (1);
			}
			pthread_mutex_unlock(&philos->rules->must_eat_count_mutex);
			if (check_death_or_full(philos, i))
				return (1);
			i++;
		}
		usleep(500);
	}
	return (1);
}
