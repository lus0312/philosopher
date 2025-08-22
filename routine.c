/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eulee <eulee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:18:03 by eulee             #+#    #+#             */
/*   Updated: 2025/08/22 22:01:36 by eulee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	precise_usleep(long long time_in_ms, t_philo *philo)
{
	long long	start_time;
	long long	end_time;
	long long	now;
	long long	diff;

	start_time = get_time_in_ms();
	end_time = start_time + time_in_ms;
	while (!check_stop(philo))
	{
		now = get_time_in_ms();
		if (now >= end_time)
			break ;
		diff = end_time - now;
		if (diff > 2)
			usleep(diff * 1000 / 2);
		else
			usleep(100);
	}
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0 || philo->id == philo->rules->nb_philo)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork", 0);
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork", 0);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork", 0);
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork", 0);
	}
}

void	eat(t_philo *philo)
{
	print_status(philo, "is eating", 0);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_eat = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	precise_usleep(philo->rules->time_to_eat, philo);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->meal_mutex);
	if (philo->id % 2 == 0 || philo->id == philo->rules->nb_philo)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

void	sleep_and_think(t_philo *philo)
{
	long long	time_to_think;
	long long	time_since_last_eat;

	print_status(philo, "is sleeping", 0);
	precise_usleep(philo->rules->time_to_sleep, philo);
	print_status(philo, "is thinking", 0);
	pthread_mutex_lock(&philo->meal_mutex);
	time_since_last_eat = get_time_in_ms() - philo->last_eat;
	pthread_mutex_unlock(&philo->meal_mutex);
	time_to_think = philo->rules->time_to_die
		- time_since_last_eat - philo->rules->time_to_eat - 10;
	if (time_to_think > 0)
		usleep(time_to_think * philo->rules->nb_philo);
	else
		usleep(100);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0 || philo->id == philo->rules->nb_philo)
		usleep(1000 * philo->id);
	while (!check_stop(philo))
	{
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
		if (check_stop(philo))
			break ;
		usleep(500 * philo->rules->nb_philo);
	}
	return (NULL);
}
