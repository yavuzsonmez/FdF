/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FdF.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysonmez <ysonmez@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 18:56:05 by ysonmez           #+#    #+#             */
/*   Updated: 2021/10/22 11:19:44 by ysonmez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H

# define FDF_H

# include "../libft/libft.h"
# include "../minilibx_macos/mlx.h"
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <math.h>

/*	Window Resolution */

# define	WIDTH				1920
# define	HEIGHT				1080

/*	Events Hook macOS keycodes */

# define	A					0
# define	D					2
# define	S					1
# define	W					13
# define	Q					12
# define	E					14
# define	MOUSE_WHEEL_UP		4
# define	MOUSE_WHEEL_DOWN	5
# define	ESCAPE				53
# define	PLUS				69
# define	MINUS				78
# define	LEFT				123
# define	RIGHT				124
# define	DOWN				125
# define	UP					126

/*	Required structs for Minilibx */

typedef struct s_vars {
	void		*mlx;
	void		*win;
}	t_vars;

typedef struct s_data {
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}	t_data;

/*	Struct for the data parsing and storing step */

typedef struct s_parse
{
	char		*str;
	char		*buf;
	size_t		row;
	size_t		col;
	char		**arr;
	size_t		size;
}	t_parse;

/*	Struct to store data related to matrix and transformed matrix */

typedef struct s_matrix {
	double		x;
	double		y;
	double		z;
	int			color;
}	t_matrix;

/*	Data related to the window and display */

typedef struct s_screen {
	char		*file;
	double		translate_x;
	double		translate_y;
	double		scale;
	int			alt;
	double		alpha;
	double		beta;
	double		theta;
}	t_screen;

/*	Data for Bresenham algorithm in order to link 2 points (line) */

typedef struct s_bresenham {
	int			z0;
	int			z1;
	int			x0;
	int			x1;
	int			y0;
	int			y1;
	int			color0;
	int			color1;
	int			sx;
	int			sy;
	int			dx;
	int			dy;
	int			err;
	int			e2;
}	t_bresenham;

/*	Main data struct to handle every other data struct */

typedef struct s_fdf {
	t_vars		vars;
	t_data		img;
	t_parse		data;
	t_screen	*screen;
	t_matrix	*matrix;
	t_matrix	*isomatrix;
}	t_fdf;

/* -------- GET_NEXT_LINE.C -------- */
/*	Buffer, struct, proto for Get_Next_Line */

# define BUFFER_SIZE	100000

typedef struct s_gnl
{
	char		*buff;
	char		*newline;
	int			r;
	int			i;
}	t_gnl;

char	*get_next_line(int fd);

/* -------- GET_DATA.C -------- */
/*	Convert store and parse the data (from matrix to array of struct) */
int		count_row_col(char *str, t_fdf *fdf);
int		store_data(char *str, t_fdf *fdf, size_t i, size_t e);
int		error_checker(int argc, char **argv, t_fdf *fdf);

/* -------- TRANSFORM_DATA.C -------- */
/*	Render routine and Applied transformations in order */
void	render(t_fdf *fdf);
void	project(t_fdf	*fdf, size_t i);
void	change_origin(t_fdf	*fdf, int origin);
void	rotate(t_fdf *fdf);
void	scale(t_fdf *fdf);
void	translate(t_fdf *fdf);

/* -------- DRAW.C -------- */
/* --------> OVERLAY.C <-------- */
/*	Bresenham algorithm for the graphic and overlay functions */

void	draw(t_fdf	*fdf);
void	overlay(t_fdf	*fdf);
void	print_zoom(t_fdf	*fdf);
void	print_translate(t_fdf	*fdf);
void	print_angles(t_fdf	*fdf);

/* -------- KEY_EVENTS.C -------- */
/*	Keypress Events */

void	close_window(t_fdf *fdf);
void	altitude(int keycode, t_fdf *fdf);
void	move(int keycode, t_fdf *fdf);
int		key_handler(int keycode, t_fdf *fdf);

/* --------> MOUSE_EVENTS.C <-------- */
/*	Mouse Events */
int		zoom(int button, int x, int y, t_fdf *fdf);
int		angle(int keycode, t_fdf *fdf);
void	rotate_x(t_fdf *fdf);
void	rotate_y(t_fdf *fdf);
void	rotate_z(t_fdf *fdf);

/* --------> UTILS.C <-------- */
/*	Freeing the memory fct and Hexa converter for colors */

void	free_data(t_fdf	*fdf);
int		from_hexa_to_dec(char *str, size_t i, int color);
void	color_fade(t_fdf *fdf, size_t i);
void	fill_colors(t_fdf *fdf);

#endif
