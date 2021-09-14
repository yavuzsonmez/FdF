/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysonmez <ysonmez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 18:55:57 by ysonmez           #+#    #+#             */
/*   Updated: 2021/09/09 13: by ysonmez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/FdF.h"

/* Printing info related to stored data */

void ft_tester(t_parse *data, t_matrix *matrix)
{

	size_t e;

	e = 0;
	while (e < (data->col * data->row))
	{

		printf("struct %zu, X : %d, Y : %d, Z : %d, Color : %d\n", e + 1, matrix[e].x, matrix[e].y, matrix[e].z, matrix[e].color);
		e++;
	}
}

/* Event hook to close the window with ESCAPE */

int	e_close(int keycode, t_vars *vars)
{
	if (keycode == ESCAPE)
	{
		mlx_destroy_image(vars->mlx, vars->win);
		mlx_destroy_window(vars->mlx, vars->win);
		ft_memfree(vars->win);
		ft_memfree(vars->mlx);
		exit(EXIT_SUCCESS);
	}
	else
		return (-1);
	return (0);
}

/* Push pixels on the img */

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

/* Draw line between points with Bresenham algorithm */

void drawline(t_data *img, t_matrix *matrix, t_parse *data, t_screen *screen, size_t i)
{
	int dx;
	int dy;
	int p;
	int x;
	int y;
	int fade;

	while (i < data->row * data->col)
	{
		dx = abs(matrix[i + 1].x) - abs(matrix[i].x);
		dy = abs(matrix[i + 1].y) - abs(matrix[i].y);
		x = matrix[i].x;
		y = matrix[i].y;
		p = 2 * dy - dx;
		fade = 0;
		while (x < matrix[i + 1].x)
		{
			if (p >= 0)
			{
				if (x >= 0 && x < screen->SCREEN_W && y >= 0 && y < screen->SCREEN_H)
					my_mlx_pixel_put(img, x, y, matrix[i].color - fade);
				y++;
				p = p + 2 * dy - 2 * dx;
			}
			else
			{
				if (x >= 0 && x < screen->SCREEN_W && y >= 0 && y < screen->SCREEN_H)
					my_mlx_pixel_put(img, x, y, matrix[i].color - fade);
				p = p + 2 * dy;
			}
			//fade += 3;
			x++;
		}
		i++;
	}

	i = 0;
	while (i < data->row * data->col - data->col)
	{
		dx = matrix[i + 1].x - matrix[i].x;
		dy = matrix[i + data->col].y - matrix[i].y;
		x = matrix[i].x;
		y = matrix[i].y;
		p = 2 * dx - dy;
		fade = 0;
		while (y < matrix[i + data->col].y)
		{
			if (p >= 0)
			{
				if (x >= 0 && x < screen->SCREEN_W && y >= 0 && y < screen->SCREEN_H)
					my_mlx_pixel_put(img, x, y, matrix[i].color - fade);
				x++;
				p = p + 2 * dx - 2 * dy;
			}
			else
			{
				if (x >= 0 && x < screen->SCREEN_W && y >= 0 && y < screen->SCREEN_H)
					my_mlx_pixel_put(img, x, y, matrix[i].color - fade);
				p = p + 2 * dx;
			}
			//fade += 3;
			y++;
		}
		i++;
	}

}


void plot_line (t_data *img, t_matrix *matrix, t_parse *data, t_screen *screen, size_t i)
{

	(void)screen;
	size_t row;

	row = 0;
	if (i == data->col - 1)
	{
		row++;
		i++;
	}
	while (i < data->row * data->col - 1)
	{
		//

		int x1 = matrix[i+1].x;
		int x0 = matrix[i].x;
		int y1 = matrix[i+1].y;
		int y0 = matrix[i].y;
		//

		int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
		int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1;
		int err = dx + dy, e2; /* error value e_xy */

		for (;;){  /* loop */
		my_mlx_pixel_put(img, x0, y0, matrix[i].color);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
		if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
	  }
	i++;
	}
}

int main(int argc, char **argv)
{
	size_t		i;
	t_vars		vars;
	t_data		img;
	//t_data		t_img;
	t_matrix	*matrix;
	t_parse		data;
	t_screen	screen;

	screen.SCREEN_W = 1920;
	screen.SCREEN_H = 1080;
	screen.SCALE = 20;
	i = 0;
	if (argc != 2)
	{
		ft_putendl_fd("Error", 1);
		return (-1);
	}
	if (ft_count_row_col(argv[1], &data) == -1)
		return (-1);
	matrix = (t_matrix *)ft_calloc(sizeof(t_matrix), (data.size + 1));
	if (matrix == NULL)
		return (-1);
	if (ft_store_data(argv[1], &data, matrix) == -1)
		return (-1);

	/* TEST PRINT */
	ft_tester(&data, matrix);
	/* END TEST */

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, screen.SCREEN_W, screen.SCREEN_H, "FdF");
	img.img = mlx_new_image(vars.mlx, screen.SCREEN_W, screen.SCREEN_H);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

/*
	int x;
	int y;
	while (i < (data.row * data.col))
	{

		x = (matrix[i].x - matrix[i].y) * cos(0.3) * screen.SCALE + 960;
		y = ((matrix[i].x + matrix[i].y) * sin(0.3) - matrix[i].z) * screen.SCALE + 540;
		if (x >= 0 && x < screen.SCREEN_W && y >= 0 && y < screen.SCREEN_H)
			my_mlx_pixel_put(&img, x, y, matrix[i].color);
		i++;
	}
*/


	ft_transform_data(&data, matrix, &screen, 0);
	plot_line (&img, matrix, &data, &screen, 0);
	//drawline(&img, matrix, &data, &screen, 0);

	mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
	mlx_hook(vars.win, 2, 1L<<0, e_close, &vars);
	//mlx_hook(vars.win, 2, 1L<<0, ft_translate, &vars);

	mlx_loop(vars.mlx);
	return (0);
}