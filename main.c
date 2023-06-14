/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mstojilj <mstojilj@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 21:16:28 by mstojilj          #+#    #+#             */
/*   Updated: 2023/06/14 08:37:12 by mstojilj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

void	putnbr(int nb, int fd)
{
	char	*base = "0123456789";
	
	if (nb / 10 > 0)
		putnbr(nb / 10, fd);
	write(fd, &base[nb % 10], 1);
}

void	parse(char *arg, int *hour, int *min)
{
	char	*h = NULL;
	char	*m = NULL;
	int		i = 0;
	int		j = 0;

	h = malloc(sizeof(arg));
	m = malloc(sizeof(arg));
	for (i = 0; arg[i] && arg[i] != 'h'; i++)
		h[i] = arg[i];
	*hour = atoi(h);
	while (arg[i] && arg[i] <= '0' || arg[i] >= '9')
		i++;
	for (; arg[i] && arg[i] != 'm'; i++)
	{
		m[j] = arg[i];
		j++;
	}
	*min = atoi(m);
	free(h);
	free(m);
}

void	add(int *sum_hour, int *sum_min, int hour, int min)
{
	*sum_hour += hour;
	if ((*sum_min + min) % 60 == 0)
	{
		*sum_hour += 1;
		*sum_min = (*sum_min + min) % 60;
	}
	else if ((*sum_min + min) / 60 == 1)
	{
		*sum_hour += (*sum_min + min) / 60;
		*sum_min = (*sum_min + min) % 60;
	}
	else
		*sum_min += min;
}

void	write_to_txt(int h, int m, int fd)
{
	putnbr(h, fd);
	write(fd, "h", 1);
	putnbr(m, fd);
	write(fd, "m", 1);
	write(fd, "\n", 1);
}

int	main(int argc, char **argv)
{
	static int	sum_hour = 0;
	static int	sum_min = 0;
	int			hour = 0;
	int			min = 0;
	int			fd;

	if (argc < 2)
	{
		printf("Error: No arguments passed.\n");
		return (1);
	}
	for (int i = 1; argv[i]; i++)
	{
		parse(argv[i], &hour, &min);
		add(&sum_hour, &sum_min, hour, min);
	}
	fd = open("hours42.txt", O_APPEND | O_RDWR | O_CREAT, 0777);
	printf("%dh%d\n", sum_hour, sum_min);
	write_to_txt(sum_hour, sum_min, fd);
	close(fd);
	return (0);
}
