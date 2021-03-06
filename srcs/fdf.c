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

/*	Screen starting data initialisation */

static void	init_screen_data(char **argv, t_fdf *fdf)
{
	fdf->screen->file = argv[1] + 10;
	fdf->screen->scale = 10;
	fdf->screen->translate_x = (WIDTH / 2) - ((fdf->data.col / 2)
			* fdf->screen->scale);
	fdf->screen->translate_y = (HEIGHT / 2) - ((fdf->data.row / 2)
			* fdf->screen->scale);
	fdf->screen->alt = 0;
	fdf->screen->alpha = 0.9;
	fdf->screen->beta = -0.8;
	fdf->screen->theta = 0.2;
}

/*	Render Routine:
*	Create the view of the graphic or re-render it after an event hook
*/

void	render(t_fdf *fdf)
{
	fdf->img.img = mlx_new_image(fdf->vars.mlx, WIDTH, HEIGHT);
	fdf->img.addr = mlx_get_data_addr(fdf->img.img, &fdf->img.bits_per_pixel,
			&fdf->img.line_length, &fdf->img.endian);
	project(fdf, 0);
	change_origin(fdf, 1);
	rotate(fdf);
	change_origin(fdf, 0);
	scale(fdf);
	translate(fdf);
	fill_colors(fdf);
	draw(fdf);
	mlx_put_image_to_window(fdf->vars.mlx, fdf->vars.win, fdf->img.img, 0, 0);
	mlx_destroy_image (fdf->vars.mlx, fdf->img.img);
	overlay(fdf);
}

/*	Window creation
*	First rendering
*/

static void	create_window(char **argv, t_fdf *fdf)
{
	fdf->screen = (t_screen *)ft_calloc(sizeof(t_screen), 1);
	fdf->isomatrix = (t_matrix *)ft_calloc(sizeof(t_matrix), fdf->data.size);
	if (fdf->screen == NULL || fdf->isomatrix == NULL)
		return ;
	init_screen_data(argv, fdf);
	fdf->vars.mlx = mlx_init();
	fdf->vars.win = mlx_new_window(fdf->vars.mlx, WIDTH, HEIGHT, "FdF");
	render(fdf);
	mlx_hook(fdf->vars.win, 2, 1L << 0, key_handler, fdf);
	mlx_hook(fdf->vars.win, 4, 1L << 2, zoom, fdf);
	mlx_loop(fdf->vars.mlx);
}

/*	Init the big data handler struct
*	Read data in the matrix file, parse the data and store it
*	Check for errors
*	Init the graphi lib and the window
*	Calculate the first view
*	Render
*/

int	main(int argc, char **argv)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)ft_calloc(sizeof(t_fdf), 1);
	if (fdf == NULL)
		return (-1);
	if (error_checker(argc, argv, fdf) == -1)
		exit(EXIT_FAILURE);
	create_window(argv, fdf);
	free_data(fdf);
	exit(EXIT_SUCCESS);
	return (0);
}
