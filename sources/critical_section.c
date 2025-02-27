/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   critical_section.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plachard <plachard@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:51:00 by plachard          #+#    #+#             */
/*   Updated: 2025/02/27 22:15:20 by plachard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	display_action(int philo_id, const char *action, t_table *table)
{
	long long	total_time;

	total_time = get_time_ms(table->start_time);
	if (total_time == -1)
		return ;
	pthread_mutex_lock(&table->mutex[PRINT]);
	pthread_mutex_lock(&table->mutex[END]);
	if (!table->end_dinner)
		printf("%lld %u %s\n", total_time, philo_id + 1, action);
	pthread_mutex_unlock(&table->mutex[END]);
	pthread_mutex_unlock(&table->mutex[PRINT]);
	return ;
}

static void	take_cutlery(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->cutlery_locks[philo->cutlery[0]]);
	display_action(philo->id, "has taken a fork", philo->table);
	if (philo->table->seats == 1)
	{
		pthread_mutex_unlock(&philo->table->cutlery_locks[philo->cutlery[0]]);
		ft_usleep(philo->table->life_time, philo->table);
		return ;
	}
	pthread_mutex_lock(&philo->table->cutlery_locks[philo->cutlery[1]]);
	display_action(philo->id, "has taken a fork/n is eating", philo->table);
	// display_action(philo->id, "is eating", philo->table);
	pthread_mutex_lock(&philo->table->mutex[CHECK_MEAL]);
	philo->last_meal = get_time_ms(philo->table->start_time);
	philo->meal_count++;
	pthread_mutex_unlock(&philo->table->mutex[CHECK_MEAL]);
	ft_usleep(philo->table->meal_time, philo->table);
	pthread_mutex_unlock(&philo->table->cutlery_locks[philo->cutlery[1]]);
	pthread_mutex_unlock(&philo->table->cutlery_locks[philo->cutlery[0]]);
}
/*
static void	eat(t_philo *philo)
{
	display_action(philo->id, "is eating", philo->table);
	pthread_mutex_lock(&philo->table->mutex[CHECK_MEAL]);
	philo->last_meal = get_time_ms(philo->table->start_time);
	philo->meal_count++;
	pthread_mutex_unlock(&philo->table->mutex[CHECK_MEAL]);
	pthread_mutex_unlock(&philo->table->cutlery_locks[philo->cutlery[1]]);
	pthread_mutex_unlock(&philo->table->cutlery_locks[philo->cutlery[0]]);
	ft_usleep(philo->table->meal_time, philo->table);
}
*/
static void	sleep_and_think(t_philo *philo)
{
	display_action(philo->id, "is sleeping", philo->table);
	ft_usleep(philo->table->sleep_time, philo->table);
	display_action(philo->id, "is thinking", philo->table);
}

void	*critical_section(void *arg)
{
	t_philo	*philo;
	bool	dinner;

	philo = (t_philo *)arg;
	if (philo->table->seats != 1 && philo->id % 2 == EVEN)
		ft_usleep(philo->table->meal_time / 3, philo->table);
	pthread_mutex_lock(&philo->table->mutex[END]);
	dinner = philo->table->end_dinner;
	pthread_mutex_unlock(&philo->table->mutex[END]);
	while (!dinner)
	{
		take_cutlery(philo);
		if (philo->table->seats == 1)
			return (NULL);
		//eat(philo);
		sleep_and_think(philo);
		pthread_mutex_lock(&philo->table->mutex[END]);
		dinner = philo->table->end_dinner;
		pthread_mutex_unlock(&philo->table->mutex[END]);
	}
	return (NULL);
}
