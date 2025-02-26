/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plachard <plachard@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 20:36:57 by plachard          #+#    #+#             */
/*   Updated: 2025/02/26 16:49:19 by plachard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/*  LIBRARIES           */
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

/*  MACRO               */
# define MAX_PHILOS 250
# define EVEN 0
# define ODD 1

/*  ENUM                */
typedef enum e_status
{
	SUCCESS,
	FAILED,
	ERR_ARG,
	ERR_MALLOC,
	ERR_SYSTEM,
	ERR_MUTEX,
	ERR_THREAD,
	UNKNOWN
}					t_status;

typedef enum e_table_mutex
{
	PRINT,
	END,
	CHECK_MEAL
}					t_table_mutex;

/*  STRUCTURES          */

typedef struct s_philo
{
	struct s_table	*table;
	pthread_t		thread;
	unsigned int	id;
	int				meal_count;
	time_t			last_meal;
	unsigned int	cutlery[2];

}					t_philo;

typedef struct s_table
{
	unsigned int	seats;
	time_t			life_time;
	time_t			meal_time;
	time_t			sleep_time;
	int				meal_count;
	bool			end_dinner;
	long long		start_time;
	pthread_mutex_t	mutex[3];
	pthread_mutex_t	*cutlery_locks;
	t_philo			*philos;
}					t_table;

/*  FUNCTION PROTOTYPES */

int					main(int ac, char **av);
int					display_error(t_status status, t_table *table);

//  parsing.c
bool				parsing(int ac, char **av);

//	setup.c
t_status			setup_table(t_table *table, int ac, char **av);


//	start_dinner.c
t_status			start_dinner(t_table *table);

//	critical_section
void				*critical_section(void *arg);

// 	utils.c
int					ft_atoi(const char *str);
void				give_cutlery(t_philo *philo);
t_status			init_cutlery_mutex(t_table *table);
long long			get_time_ms(t_table *table);
void				ft_usleep(size_t milliseconds, t_table *table);

t_status	set_start_time(t_table *table);
#endif