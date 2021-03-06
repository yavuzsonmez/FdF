/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysonmez <ysonmez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/28 11:44:37 by ysonmez           #+#    #+#             */
/*   Updated: 2021/10/22 10:55:28 by ysonmez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/FdF.h"

/*	Get the adress where the color information of a point start
*	and send it to a converter in order to get the integer value
*/

static int	get_color(char *str)
{
	size_t	i;
	int		color;

	i = 0;
	color = 16777215;
	while (str[i] != '\0' && str[i] != ',')
		i++;
	if (str[i++] == ',')
		color = from_hexa_to_dec(str + i, 0, 0);
	return (color);
}

/*	First read in the file descriptor (Matrix)
*	Count the number of column and row
*/

int	count_row_col(char *str, t_fdf *fdf)
{
	size_t		i;
	int			fd;

	i = 0;
	fd = open(str, O_RDONLY);
	if (fd < 0)
		return (-1);
	fdf->data.buf = get_next_line(fd);
	fdf->data.str = ft_strtrim(fdf->data.buf, "\n");
	fdf->data.arr = ft_split(fdf->data.str, ' ');
	while (fdf->data.arr[i++] != NULL)
		fdf->data.col++;
	ft_memfree(fdf->data.str);
	ft_memfreeall((void *)fdf->data.arr);
	while (fdf->data.buf != NULL)
	{
		fdf->data.row++;
		ft_memfree(fdf->data.buf);
		fdf->data.buf = get_next_line(fd);
	}
	fdf->data.size = fdf->data.row * fdf->data.col;
	close(fd);
	return (0);
}

/*	Second read in the file descriptor (Matrix)
*	Use thenumber of col and row to store all points coordinate
*	in an array of struct matrix[i] with, x, y, z, color property
*/

int	store_data(char *str, t_fdf *fdf, size_t i, size_t e)
{
	size_t		k;
	int			fd;

	fd = open(str, O_RDONLY);
	if (fd < 0)
		return (-1);
	while (i < fdf->data.row)
	{
		fdf->data.buf = get_next_line(fd);
		fdf->data.arr = ft_split(fdf->data.buf, ' ');
		k = 0;
		while (k < fdf->data.col)
		{
			fdf->matrix[e].x = k;
			fdf->matrix[e].y = i;
			fdf->matrix[e].z = ft_atoi(fdf->data.arr[k]);
			fdf->matrix[e++].color = get_color(fdf->data.arr[k++]);
		}
		ft_memfree((void *)fdf->data.buf);
		ft_memfreeall((void **)fdf->data.arr);
		i++;
	}
	close(fd);
	return (0);
}

/*	Read, parse and store the data related to the matrix
*	Check if there are one or multiple errors during the process
*	or in the inputs
*/

int	error_checker(int argc, char **argv, t_fdf *fdf)
{
	int	error;

	error = 0;
	if (argc != 2)
		error = 1;
	if (count_row_col(argv[1], fdf) == -1 && error == 0)
		error = 1;
	fdf->matrix = (t_matrix *)ft_calloc(sizeof(t_matrix), fdf->data.size);
	if (fdf->matrix == NULL)
		error = 1;
	if (store_data(argv[1], fdf, 0, 0) == -1 && error == 0)
		error = 1;
	if (error == 1)
	{
		free_data(fdf);
		ft_putendl_fd("Error", 1);
		return (-1);
	}
	else
		return (0);
}
