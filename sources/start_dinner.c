/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_dinner.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderison <aderison@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 00:22:07 by plachard          #+#    #+#             */
/*   Updated: 2025/02/26 18:54:03 by aderison         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_death(t_table *table)
{
	unsigned int	i;
	long long		current_time;

	current_time = get_time_ms(table);
	if (current_time == -1)
		return (display_error(ERR_SYSTEM, table), false);
	i = 0;
	while (i < table->seats)
	{
		if ((current_time - table->philos[i].last_meal) >= table->life_time)
		{
			// AFFICHAGE : print_death(table, philo id, death)
			pthread_mutex_lock(&table->mutex[PRINT]);
			printf("%lld %u died\n", current_time - table->start_time, table->philos[i].id + 1);
			pthread_mutex_unlock(&table->mutex[PRINT]);
			// STOP SIM : end_dinner(table)
			pthread_mutex_lock(&table->mutex[END]);
			table->end_dinner = true;
			pthread_mutex_unlock(&table->mutex[END]);
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
	// STOP SIM : end_dinner(table)
	pthread_mutex_lock(&table->mutex[END]);
	table->end_dinner = true;
	pthread_mutex_unlock(&table->mutex[END]);
	return (true);
}

static void	check_philo(t_table *table)
{
	bool	dinning;

	dinning = true;
	while (dinning)
	{
		if (check_death(table) || check_meal(table))
			dinning = false;
		// ft_usleep(100, table);
	}
}

static t_status	create_philo(t_table *table)
{
	unsigned int	i;

	table->start_time = get_time_ms(table) + (table->seats * 2 *10);
	i = 0;
	while (i < table->seats)
	{
		table->philos[i].last_meal = get_time_ms(table);
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
	check_philo(table);
	i = 0;
	while (i < table->seats)
	{
		if (pthread_join(table->philos[i].thread, NULL) != SUCCESS)
			return (ERR_THREAD);
		++i;
	}
	return (SUCCESS);
}
