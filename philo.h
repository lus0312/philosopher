#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>

typedef struct s_rules
{
	int				nb_philo;
	int				must_eat;
	int				is_dead;
	long long		start_time;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*forks;
}	t_rules;

typedef struct s_philo
{
	int				id;
	int				eat_count;
	int				eat_flag;
	long long		last_eat;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
	pthread_t		thread;
	t_rules			*rules;
}	t_philo;

int		init_rules(t_rules *rules, int ac, char **av);
int		init_philo(t_rules *rules, t_philo **philos);
void	*philo_routine(void *arg);
void	handle_one_philo(t_philo *philo);
void	print_status(t_philo *philo, char *msg, int force);
int		monitoring(void *arg);
void	free_all(t_philo *philos, t_rules *rules);
int		ft_atoi(const char *str);
long long	get_time_in_ms(void);
void	free_rules(t_rules *rules);
void	free_philos(t_philo **philos, int i);
void	free_forks(t_rules *rules, int i);

#endif
