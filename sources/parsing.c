/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plachard <plachard@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 23:50:54 by plachard          #+#    #+#             */
/*   Updated: 2025/02/25 01:12:57 by plachard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_int(char *str)
{
	unsigned long long	nb;

	if (str == NULL || *str == '\0')
		return (false);
	if (*str == '0' && *(str + 1) != '\0')
		return (false);
	nb = 0;
	while (*str != '\0')
	{
		if (!(*str >= '0' && *str <= '9'))
			return (false);
		nb = nb * 10 + (*str - '0');
		if (nb > INT_MAX)
			return (false);
		str++;
	}
	return (true);
}

bool	parsing(int ac, char **av)
{
	int	i;
	int	nb;

	if (ac != 5 && ac != 6)
	{
		return (false);
	}
	i = 0;
	while (++i < ac)
	{
		if (!is_int(av[i]))
			return (false);
		nb = ft_atoi(av[i]);
		if (i == 1 && (nb <= 0 || nb > MAX_PHILOS))
			return (false);
		if (i != 1 && nb == -1)
			return (false);
	}
	return (true);
}
