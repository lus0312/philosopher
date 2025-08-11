/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eulee <eulee@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:21:43 by eulee             #+#    #+#             */
/*   Updated: 2025/08/11 14:50:31 by eulee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(const char *str)
{
	int	len;

	len = 0;
	while (*str && *str != '\0')
	{
		len++;
		str++;
	}
	return (len);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	cnt;

	cnt = 1;
	result = 0;
	while (*str == 32 || (*str >= 8 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			cnt *= -1;
		str++;
	}
	if (*str >= '0' && *str <= '9')
	{
		while (*str >= '0' && *str <= '9')
		{
			result = result * 10 + (*str - '0');
			str++;
		}
		return (cnt * result);
	}
	else
		return (0);
}

long long	get_time_in_ms(void)
{
	struct timeval	now;
	
	if (gettimeofday(&now, NULL) != 0)
		return (-1);
	return (now.tv_sec * 1000LL + (now.tv_usec + 500) / 1000);
}

void	print_status(t_philo *philo, char *msg, int force)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->rules->print_mutex);
	pthread_mutex_lock(&philo->rules->dead_mutex);
	if (philo->rules->is_dead && !force)
	{
		pthread_mutex_unlock(&philo->rules->dead_mutex);
		pthread_mutex_unlock(&philo->rules->print_mutex);
		return ;
	}
	timestamp = get_time_in_ms() - philo->rules->start_time;
	
	if (!(philo->rules->is_dead && !force))
		printf("%lld %d %s\n", timestamp, philo->id, msg);
	pthread_mutex_unlock(&philo->rules->dead_mutex);
	pthread_mutex_unlock(&philo->rules->print_mutex);
}
