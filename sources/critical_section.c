/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   critical_section.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderison <aderison@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:51:00 by plachard          #+#    #+#             */
/*   Updated: 2025/02/27 23:40:52 by aderison         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	display_action(int philo_id, const char *action, t_table *table)
{
	long long	time;

	time = timestamp() - table->start_time;
	pthread_mutex_lock(&table->mutex[PRINT]);
	if (!is_end(table))
		printf("%lld %u %s\n", time, philo_id + 1, action);
	pthread_mutex_unlock(&table->mutex[PRINT]);
	return ;
}

static void	take_cutlery(t_philo *philo)
{
	display_action(philo->id, "has taken a fork", philo->table);
	pthread_mutex_lock(&philo->table->cutlery_locks[philo->cutlery[0]]);
	if (philo->table->seats == 1)
	{
		pthread_mutex_unlock(&philo->table->cutlery_locks[philo->cutlery[0]]);
		ft_usleep(philo->table->life_time);
		return ;
	}
	display_action(philo->id, "has taken a fork", philo->table);
	pthread_mutex_lock(&philo->table->cutlery_locks[philo->cutlery[1]]);
}

static void	eat(t_philo *philo)
{
	take_cutlery(philo);
	display_action(philo->id, "is eating", philo->table);
	pthread_mutex_lock(&philo->table->mutex[CHECK_MEAL]);
	philo->last_meal = timestamp();
	philo->meal_count++;
	ft_usleep(philo->table->meal_time);
	pthread_mutex_unlock(&philo->table->mutex[CHECK_MEAL]);
	pthread_mutex_unlock(&philo->table->cutlery_locks[philo->cutlery[1]]);
	pthread_mutex_unlock(&philo->table->cutlery_locks[philo->cutlery[0]]);
}

static void	sleep_and_think(t_philo *philo)
{
	display_action(philo->id, "is sleeping", philo->table);
	ft_usleep(philo->table->sleep_time);
	display_action(philo->id, "is thinking", philo->table);
}

void	*critical_section(void *arg)
{
	t_philo	*philo;
	bool	dinner;

	philo = (t_philo *)arg;
	if (philo->table->seats != 1 && philo->id % 2 == EVEN)
		ft_usleep(philo->table->meal_time / 3);
	dinner = is_end(philo->table);
	while (!dinner)
	{
		eat(philo);
		if (philo->table->seats == 1)
			return (NULL);
		sleep_and_think(philo);
		dinner = is_end(philo->table);
	}
	return (NULL);
}
