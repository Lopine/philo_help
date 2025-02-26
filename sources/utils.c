/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plachard <plachard@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:33:59 by plachard          #+#    #+#             */
/*   Updated: 2025/02/26 17:50:30 by plachard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	nb;

	sign = 1;
	nb = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
		sign = 44 - *str++;
	while (*str >= '0' && *str <= '9')
		nb = nb * 10 + (*str++ - '0');
	return (sign * nb);
}

long long	get_time_ms(t_table *table)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) != SUCCESS)
	{
		pthread_mutex_lock(&table->mutex[END]);
		table->end_dinner = true;
		pthread_mutex_unlock(&table->mutex[END]);
		// pthread_mutex_lock(&table->mutex[PRINT]);
		// display_error(ERR_SYSTEM, table);
		// pthread_mutex_unlock(&table->mutex[PRINT]);
		return (-1);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
void	ft_usleep(size_t milliseconds, t_table *table)
{
	long long	start;

	start = get_time_ms(table);
	if (start == -1)
		return ;
	while ((get_time_ms(table) - start) < (long long)milliseconds)
		usleep(100);
}

void	give_cutlery(t_philo *philo)
{
	if (philo->id % 2 == EVEN)
	{
		philo->cutlery[0] = philo->id;
		philo->cutlery[1] = (philo->id + 1) % philo->table->seats;
	}
	if (philo->id % 2 == ODD)
	{
		philo->cutlery[0] = (philo->id + 1) % philo->table->seats;
		philo->cutlery[1] = philo->id;
	}
}

t_status	init_cutlery_mutex(t_table *table)
{
	unsigned int	i;

	table->cutlery_locks = malloc(sizeof(pthread_mutex_t) * table->seats);
	if (!table->cutlery_locks)
		return (ERR_MALLOC);
	i = 0;
	while (i < table->seats)
	{
		if (pthread_mutex_init(&table->cutlery_locks[i], 0) != SUCCESS)
			return (ERR_MUTEX);
		++i;
	}
	return (SUCCESS);
}