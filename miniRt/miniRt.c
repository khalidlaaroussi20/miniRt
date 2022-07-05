/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klaarous <klaarous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/26 14:45:49 by klaarous          #+#    #+#             */
/*   Updated: 2022/07/05 16:46:30 by klaarous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/miniRt.h"

void	initializemlx(t_parameters *param)
{
	param->mlx_ptre = mlx_init();
	param->win_ptre = mlx_new_window(param->mlx_ptre, \
		param->camera.hsize, param->camera.vsize, "MINIRT");
	param->img_ptre = mlx_new_image(param->mlx_ptre, \
						param->camera.hsize, param->camera.vsize);
	param->address = mlx_get_data_addr(param->img_ptre, \
					&param->bitsperpixel, &param->linesize, &param->endian);
}

int	esc_hook(int button, void *param)
{
	if (button == 53 || button == 17)
	{
		exit(0);
	}
	return (0);
}

void	free_ressources(t_world *world)
{
	t_object	*objects;
	t_light		*lights =  world->light;

	lights =  world->light;
	objects = world->objects;
	while (objects)
	{
		free_matrix(objects->inverse_transformation);
		free_matrix(objects->transformation);
		if (objects->material.pattern)
		{
			free_matrix(objects->material.pattern->transformation);
			free_matrix(objects->material.pattern->inverse_transformation);
			free(objects->material.pattern);
		}
		free(objects->object);
		t_object *next = objects ->next;
		free(objects);
		objects = next;
	}
	while (lights)
	{
		t_light *next_light = lights ->next;
		free(lights);
		lights = next_light;
	}
}

int	main(int argc, char **argv)
{
	t_camera		camera;
	t_world			world;
	t_parameters	param;
	int				fd;

	if (argc != 2)
		failure_exit("Number argument not valid\n");
	check_extention(argv[1]);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		failure_exit("fail to open file\n");
	parse_file(&world, &camera, fd);
	close (fd);
	if (world.light == NULL || world.objects == NULL)
		failure_exit("Empty file\n");
	param.camera = camera;
	initializemlx(&param);
	render(camera, world, &param);
	free_ressources(&world);
	free_matrix(camera.inverse_transform);
	free_matrix(camera.transform);
	system("leaks miniRt");
	mlx_put_image_to_window(param.mlx_ptre, \
			param.win_ptre, param.img_ptre, 0, 0);
	mlx_hook(param.win_ptre, 02, 0L, esc_hook, &param);
	mlx_hook(param.win_ptre, 17, 0L, exitit, &param);
	mlx_loop(param.mlx_ptre);
	return (0);
}
