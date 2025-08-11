/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eulee <eulee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:18:03 by eulee             #+#    #+#             */
/*   Updated: 2025/08/11 16:37:24 by eulee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_stop(void *arg)
{
	t_philo	*philo;
	int		is_dead;
	
	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->rules->dead_mutex);
	is_dead = philo->rules->is_dead;
	pthread_mutex_unlock(&philo->rules->dead_mutex);
	return (is_dead);
}

void	precise_usleep(long long time_in_ms, t_philo *philo)
{
	long long	start_time;

	start_time = get_time_in_ms();
	while (!check_stop(philo))
	{
		if((get_time_in_ms() - start_time) > time_in_ms)
			break;
		usleep(100);
	}
}

//void precise_usleep(long long time_in_ms, t_philo *philo)
//{
//    long long start_time;
//    long long end_time;
//    long long now;
//    long long diff;

//    start_time = get_time_in_ms();
//    end_time = start_time + time_in_ms;
//    while (!check_stop(philo))
//    {
//        now = get_time_in_ms();
//        if (now >= end_time)
//            break;
//        diff = end_time - now;
//        if (diff > 2) // 2ms 이상 남으면 그만큼 자고
//            usleep(diff * 1000 / 2);
//        else
//            usleep(100); // 아주 짧게 반복
//    }
//}

void	handle_one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo,"has taken a fork",0);
	pthread_mutex_unlock(philo->left_fork);
	precise_usleep(philo->rules->time_to_die, philo);
	print_status(philo, "died",1);
}

void take_forks(t_philo *philo)
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

void eat(t_philo *philo)
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
	print_status(philo, "is sleeping",0);
	precise_usleep(philo->rules->time_to_sleep, philo);
	print_status(philo, "is thinking",0);
}


void	*philo_routine(void *arg)
{
	t_philo *philo;
	philo = (t_philo *)arg;

	if (philo->id % 2 == 0 || philo->id == philo->rules->nb_philo) // 짝수 철학자 딜레이 줘서 데드락 방지
		usleep(1000);
	while (!check_stop(philo))
	{
		take_forks(philo);
		eat(philo);
		sleep_and_think(philo);
		if (check_stop(philo))
			break;
		usleep(1000);
	}
	return (NULL);
}
