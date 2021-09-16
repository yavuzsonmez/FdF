/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysonmez <ysonmez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/15 15:10:50 by ysonmez           #+#    #+#             */
/*   Updated: 2021/09/16 14:40:51 by ysonmez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/FdF.h"

/* Calculate the memory offset using the line length */

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

/* Draw line between points with Bresenham algorithm */


static void bresenham (int x0, int x1, int y0, int y1, int color, t_screen *screen, t_data *img)
{

		int sx;
		int sy;

		int dx =  abs (x1 - x0);
		if (x0 < x1)
			sx = 1;
		else
			sx = -1;
		int dy = -abs (y1 - y0);
		if (y0 < y1)
			sy = 1;
		else
			sy = -1;
		int err = dx + dy;
		int e2;
		while (1)
		{

			if (x0 >= 0 && x0 < screen->SCREEN_W && y0 >= 0 && y0 < screen->SCREEN_H)
				my_mlx_pixel_put(img, x0, y0, color);
			if (x0 == x1 && y0 == y1)
				break;
			e2 = 2 * err;
			if (e2 >= dy)
			{
				err += dy;
				x0 += sx;
			}
			if (e2 <= dx)
			{
				err += dx;
				y0 += sy;
			}
		}
}


link_x

link_y

void draw(t_data *img, t_matrix *matrix, t_parse *data, t_screen *screen, t_matrix *isomatrix)
{
	size_t	i;
	int		x0;
	int		x1;
	int		y0;
	int		y1;
	int color;

	i = 0;
	while (i < data->size - 1)
	{
		if (matrix[i].y != matrix[i + 1].y)
			i++;
		x0 = isomatrix[i].x;
		x1 = isomatrix[i + 1].x;
		y0 = isomatrix[i].y;
		y1 = isomatrix[i + 1].y;
		color = isomatrix[i].color;
		bresenham (x0, x1, y0, y1, color, screen, img);
		i++;
	}
	i = 0;
	while (i < (data->size - data->col))
	{
		x0 = isomatrix[i].x;
		x1 = isomatrix[i + data->col].x;
		y0 = isomatrix[i].y;
		y1 = isomatrix[i + data->col].y;
		color = isomatrix[i].color;
		bresenham (x0, x1, y0, y1, color, screen, img);
		i++;
	}
}
