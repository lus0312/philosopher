/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eulee <eulee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:57:17 by eulee             #+#    #+#             */
/*   Updated: 2025/08/11 22:07:43 by eulee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	monitoring(void *arg)
{
	t_philo	*philos;
	int			i;
	int			nb;
	int			must_eat_count;
	long long	now;

	philos = (t_philo *)arg;
	must_eat_count = 0;
	nb = philos[0].rules->nb_philo;
	while (1)
	{
		i = 0;
		while (i < nb)
		{
            now = get_time_in_ms();
			pthread_mutex_lock(&philos[i].meal_mutex);
			if (now - philos[i].last_eat >= philos[i].rules->time_to_die)
			{
				pthread_mutex_lock(&philos[i].rules->dead_mutex);	//is_dead 값 바꿀 때 mutex
				philos[i].rules->is_dead = 1;
				pthread_mutex_unlock(&philos[i].rules->dead_mutex);
				print_status(&philos[i], "died", 1);
				pthread_mutex_unlock(&philos[i].meal_mutex);
				return (1);
			}
            pthread_mutex_unlock(&philos[i].meal_mutex);
			if (philos[i].rules->must_eat != -1 && philos[i].eat_count == philos[i].rules->must_eat)
			{
				if (philos[i].eat_flag != 1)
				{
					must_eat_count++;
					philos[i].eat_flag = 1;
					if (must_eat_count == philos[0].rules->nb_philo)
					{
						pthread_mutex_unlock(&philos[i].meal_mutex);
				        pthread_mutex_lock(&philos[i].rules->dead_mutex);	//is_dead 값 바꿀 때 mutex
				        philos[i].rules->is_dead = 1;
				        pthread_mutex_unlock(&philos[i].rules->dead_mutex);
						return (1);
					}
				}
				if (must_eat_count == philos[0].rules->nb_philo)
				{
					pthread_mutex_unlock(&philos[i].meal_mutex);
					return (1);
				}
			}
			pthread_mutex_unlock(&philos[i].meal_mutex);
			i++;
		}
		usleep(500);
	}	
}
