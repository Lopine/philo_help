/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plachard <plachard@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:48:32 by plachard          #+#    #+#             */
/*   Updated: 2025/02/26 13:38:22 by plachard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	clean_table(t_table *table)
{
	unsigned int	i;

	i = 0;
	while (i < 3)
		if (&table->mutex[i])
			pthread_mutex_destroy(&table->mutex[i++]);
	i = 0;
	while (i < table->seats)
	{
		if (&table->cutlery_locks[i])
			pthread_mutex_destroy(&table->cutlery_locks[i]);
		++i;
	}
	if (&table->cutlery_locks)
		free(table->cutlery_locks);
	if (&table->philos)
		free(table->philos);
	return ;
}

int	display_error(t_status status, t_table *table)
{
	if (status == ERR_ARG)
		write(2, "./philo <number_of_philosophers> <time_to_die> <time_to_eat> \
<time_to_sleep> [number_of_times_each_philosopher_must_eat]\n", 122);
	else if (status == ERR_MALLOC)
		write(2, "Error: Memory allocation failed\n", 32);
	else if (status == ERR_SYSTEM)
		write(2, "Error: System call failed\n", 27);
	else
		write(2, "Error: Unknown error\n", 22);
	if (table)
		clean_table(table);
	return ((int)status);
}

int	main(int ac, char **av)
{
	t_table		table;
	t_status	status;

	if (!parsing(ac, av))
		return (display_error(ERR_ARG, NULL));
	status = setup_table(&table, ac, av);
	if (status != SUCCESS)
		return (display_error(status, &table));
	status = start_dinner(&table);
	if (status != SUCCESS)
		return (display_error(status, &table));
	clean_table(&table);
	return ((int)status);
}
