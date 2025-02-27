/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aderison <aderison@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 00:32:18 by plachard          #+#    #+#             */
/*   Updated: 2025/02/27 23:41:02 by aderison         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_table(t_table *table, int ac, char **av)
{
	table->seats = ft_atoi(av[1]);
	table->life_time = ft_atoi(av[2]);
	table->meal_time = ft_atoi(av[3]);
	table->sleep_time = ft_atoi(av[4]);
	if (ac == 6)
		table->meal_count = ft_atoi(av[5]);
	else
		table->meal_count = -1;
	table->end_dinner = false;
	return ;
}

static t_status	init_dinner_mutex(t_table *table)
{
	t_status	status;

	status = init_cutlery_mutex(table);
	if (status != SUCCESS)
		return (status);
	if (pthread_mutex_init(&table->mutex[END], 0) != SUCCESS)
		return (ERR_MUTEX);
	if (pthread_mutex_init(&table->mutex[PRINT], 0) != SUCCESS)
		return (ERR_MUTEX);
	if (pthread_mutex_init(&table->mutex[CHECK_MEAL], NULL) != SUCCESS)
		return (ERR_MUTEX);
	return (SUCCESS);
}

static t_status	init_philos(t_table *table)
{
	unsigned int	i;

	table->philos = malloc(sizeof(t_philo) * table->seats);
	if (!table->philos)
		return (ERR_MALLOC);
	i = 0;
	while (i < table->seats)
	{
		table->philos[i].table = table;
		table->philos[i].id = i;
		table->philos[i].meal_count = 0;
		table->philos[i].last_meal = 0;
		give_cutlery(&table->philos[i]);
		++i;
	}
	return (SUCCESS);
}

t_status	setup_table(t_table *table, int ac, char **av)
{
	t_status	status;

	init_table(table, ac, av);
	status = init_philos(table);
	if (status != SUCCESS)
		return (status);
	status = init_dinner_mutex(table);
	if (status != SUCCESS)
		return (status);
	return (SUCCESS);
}
