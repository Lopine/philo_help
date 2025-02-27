/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_dinner.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderison <aderison@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 00:22:07 by plachard          #+#    #+#             */
/*   Updated: 2025/02/27 23:41:05 by aderison         ###   ########.fr       */
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
	bool			is_died;

	is_died = false;
	current_time = timestamp();
	if (current_time == -1)
		return (display_error(ERR_SYSTEM, table), false);
	i = 0;
	while (i < table->seats)
	{
		pthread_mutex_lock(&table->mutex[CHECK_MEAL]);
		is_died = (current_time
				- table->philos[i].last_meal) >= table->life_time + 1;
		pthread_mutex_unlock(&table->mutex[CHECK_MEAL]);
		if (is_died)
		{
			display_action(table->philos[i].id, "died", table);
			end_dinner(table);
			return (true);
		}
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
	long long	i;

	table->start_time = timestamp();
	i = 0;
	while (i < table->seats)
	{
		table->philos[i].last_meal = table->start_time;
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
