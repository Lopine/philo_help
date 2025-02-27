/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_dinner.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plachard <plachard@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 00:22:07 by plachard          #+#    #+#             */
/*   Updated: 2025/02/27 21:44:43 by plachard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	end_dinner(t_table *table)
{
	pthread_mutex_lock(&table->mutex[END]);
	table->end_dinner = true;
	pthread_mutex_unlock(&table->mutex[END]);
	return ;
}

static bool	check_death(t_table *table)
{
	unsigned int	i;
	long long		current_time;

	current_time = get_time_ms(table->start_time);
	if (current_time == -1)
		return (display_error(ERR_SYSTEM, table), false);
	i = 0;
	while (i < table->seats)
	{
		pthread_mutex_lock(&table->mutex[CHECK_MEAL]);
		if ((current_time - table->philos[i].last_meal) >= table->life_time + 1)
		{
			display_action(table->philos[i].id, "died", table);
			end_dinner(table);
			pthread_mutex_unlock(&table->mutex[CHECK_MEAL]);
			return (true);
		}
		pthread_mutex_unlock(&table->mutex[CHECK_MEAL]);
		++i;
	}
	return (false);
}

static bool	check_meal(t_table *table)
{
	unsigned int	i;

	i = 0;
	pthread_mutex_lock(&table->mutex[CHECK_MEAL]);
	if (table->meal_count == -1)
	{
		pthread_mutex_unlock(&table->mutex[CHECK_MEAL]);
		return (false);
	}
	while (i < table->seats)
	{
		if (table->philos[i++].meal_count < table->meal_count)
		{
			pthread_mutex_unlock(&table->mutex[CHECK_MEAL]);
			return (false);
		}
	}
	pthread_mutex_unlock(&table->mutex[CHECK_MEAL]);
	end_dinner(table);
	return (true);
}

static t_status	create_philo(t_table *table)
{
	unsigned int	i;

	gettimeofday(&table->start_time, NULL);
	i = 0;
	while (i < table->seats)
	{
		table->philos[i].last_meal = get_time_ms(table->start_time);
		if (pthread_create(&table->philos[i].thread, NULL, critical_section,
				&table->philos[i]) != SUCCESS)
			return (ERR_THREAD);
		++i;
	}
	return (SUCCESS);
}

t_status	start_dinner(t_table *table)
{
	unsigned int	i;
	t_status		status;

	status = create_philo(table);
	if (status != SUCCESS)
		return (status);
	while (!check_death(table) && !check_meal(table))
		;
	i = 0;
	while (i < table->seats)
	{
		if (pthread_join(table->philos[i].thread, NULL) != SUCCESS)
			return (ERR_THREAD);
		++i;
	}
	return (SUCCESS);
}
