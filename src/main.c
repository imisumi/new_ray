/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/09/12 23:39:30 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"
// #include "main.h"
#include <float.h>
#include <pthread.h>
static mlx_image_t* image;
int	total_frames = 0;
double previousTime = 0.0;

// -----------------------------------------------------------------------------


int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void put_pixel(mlx_image_t* image, uint32_t x, uint32_t y, uint32_t color)
{
	if (x >= 0 && x < image->width && y >= 0 && y < image->height)
		mlx_put_pixel(image, x, y, color);
}


// int next = 0;

uint32_t vec4_to_color(t_vec4 c)
{
	uint8_t r = (uint8_t)(c.x * 255.0);
	uint8_t g = (uint8_t)(c.y * 255.0);
	uint8_t b = (uint8_t)(c.z * 255.0);
	uint8_t a = (uint8_t)(c.w * 255.0);
	return (ft_pixel(r, g, b, a));
}

float	random_value(uint32_t *state)
{
	*state = *state * 747796405 + 2891336453;
	uint32_t resuls = ((*state >> ((*state >> 28) + 4)) ^ *state) * 277803737;
	resuls = (resuls >> 22) ^ resuls;
	// printf("%u\n", resuls);
	// printf("float max = %f\n", FLT_MAX);
	return ((float)resuls / UINT32_MAX);
}

void ft_hook(void* param)
{
	mlx_t* mlx = param;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
		image->instances[0].y -= 5;
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
		image->instances[0].y += 5;
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
		image->instances[0].x -= 5;
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		image->instances[0].x += 5;
}

void	frame_times(void *arg)
{
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - previousTime;
	double frameTimeMs = deltaTime * 1000.0;
	//! use this one
	// printf("\rframeTimeMs: %.2f ms    frames: %d	accumulated frames: %d", frameTimeMs, total_frames, accumulated_frames);
	printf("\rframeTimeMs: %.2f ms    frames: %d", frameTimeMs, total_frames);


	
	// printf("\rFPS: %.2f    ",1000 / frameTimeMs);
	fflush(stdout);
	previousTime = currentTime;

	total_frames++;
}

void	set_render_zones(t_utils *utils)
{
	if (MT)
	{
		for (int i = 0; i < THREADS; i++)
		{
			//! Vertical lines
			int block_width = WIDTH / THREADS;
			int x_start = i * block_width;
			int x_end = (i + 1) * block_width;
	
			utils->blocks[i].x_start = x_start;
			utils->blocks[i].x_end = x_end;
			utils->blocks[i].y_start = 0;
			utils->blocks[i].y_end = HEIGHT;

			//! Horizontal lines
			// int block_height = HEIGHT / THREADS;
			// int y_start = i * block_height;
			// int y_end = (i + 1) * block_height;

			// blocks[i].x_start = 0;
			// blocks[i].x_end = WIDTH;
			// blocks[i].y_start = y_start;
			// blocks[i].y_end = y_end;
		}
	}
	else
	{
		utils->blocks[0].x_start = 0;
		utils->blocks[0].x_end = WIDTH;
		utils->blocks[0].y_start = 0;
		utils->blocks[0].y_end = HEIGHT;
	}
}

t_vec4	per_pixel(t_ray ray, t_scene s, t_vec2 xy, uint32_t *rngState, t_vec2 coord)
{


	t_hitinfo closest_hit = {0};

	closest_hit.distance = FLT_MAX;

	int	num_spheres = 1;
	int	i = 0;

	float *f;

	// printf("num_spheres:\n");
	// vec_init(&f, 50, sizeof(float));
	// printf("float = %d\n", array_length(&f));
	// // vec_init(&s.spheres, 4, sizeof(t_sphere));
	// printf("%d\n", array_length(&s.spheres));
	// exit(0);
	while (i < array_length(&s.spheres))
	{
		t_sphere sphere = s.spheres[i];
		t_hitinfo hitinfo = sphere_intersection(ray, sphere);
		if (hitinfo.hit && hitinfo.distance < closest_hit.distance)
		{
			closest_hit = hitinfo;
			closest_hit.material = sphere.material;
			// closest_hit.material.color = vec4_new(1.0f, 0.0f, 0.0f, 1.0f);
		}
		i++;
	}
	return (closest_hit.material.color);

	









	//print ray direction
	// printf("%f, %f, %f\n", ray.direction.x, ray.direction.y, ray.direction.z);
	// t_hitinfo hitinfo = sphere_intersection(ray, vec3_new(0.0f, 0.0f, 0.0f), 0.5f);
	// if (hitinfo.hit)
	// 	return (vec4_new(1.0f, 0.0f, 0.0f, 1.0f));
	// else
	// 	return (vec4_new(0.0f, 1.0f, 0.0f, 1.0f));
	// return (vec4_new(rr, g, b, 1.0f));
	// return (vec4_new(1.0f, 0.0f, 0.0f, 1.0f));
	return (vec4_new(1.0f, 0.0f, 0.0f, 1.0f));
}

void	*render(void *param)
{
	t_thread_data threadData = *(t_thread_data *)param;
	t_data *data = threadData.data;
	int	index = threadData.threadIndex;
	uint32_t color;
	uint32_t rngState;

	t_ray	ray;
	ray.origin = data->scene.camera.position;
	// printf("pos x: %f\n", data->scene.camera.position.x);

	for (int y = data->utils.blocks[index].y_start; y <data->utils.blocks[index].y_end; y++)
	{
		for (int x = data->utils.blocks[index].x_start; x < data->utils.blocks[index].x_end; x++)
		{
			t_vec2 coord = {(float)x / (float)(WIDTH), (float)y / (float)(HEIGHT)};
			t_vec2 numPixels = vec2_new((float)WIDTH, (float)HEIGHT);
			t_vec2 pixelCoord = vec2_mul(coord, numPixels);
			uint32_t pixelIndex = pixelCoord.x + pixelCoord.y * numPixels.x;
			rngState = pixelIndex + total_frames * 719393;
			// for (int i = 0; i < 5; i++)
			// {
			// 	float	r = random_value(&rngState);
			// 	float	g = random_value(&rngState);
			// 	float	b = random_value(&rngState);
			// 	color = vec4_to_color(vec4_new(r, g, b, 1.0f));
			// }

			ray.direction = data->scene.camera.ray_dir[x + y * WIDTH];
			// printf("%f, %f, %f\n", ray.direction.x, ray.direction.y, ray.direction.z);

			t_vec4 col = per_pixel(ray, data->scene, vec2_new(x, y), &rngState, coord);

			color = vec4_to_color(col);
			for (int i = 0; i < PIXEL_SIZE; i++)
			{
				for (int j = 0; j < PIXEL_SIZE; j++)
				{
					put_pixel(image, (x * PIXEL_SIZE) + i, \
						((HEIGHT - y - 1) * PIXEL_SIZE) + j, \
						color);
				}
			}
		}
		// exit(0);
	}
	// exit(0);
}

void	render_loop(void *param)
{
	t_thread_data threadData[THREADS];
	t_data *data = param;
	movement(data);
	int	i;
	// t_scene scene = data->scene;
	// t_ray	ray;
	// ray.origin = scene.camera.position;
	set_render_zones(&data->utils);
	i = 0;
	if (MT)
	{
		while (i < THREADS)
		{
			threadData[i].data = data; // Store a pointer to the t_data structure
			threadData[i].threadIndex = i; // Store the thread index
			pthread_create(&data->utils.threads[i], NULL, render, &threadData[i]);
			i++;
		}
		i = 0;
		while (i < THREADS)
		{
			pthread_join(data->utils.threads[i], NULL);
			i++;
		}
	}
	else
	{
		threadData[0].data = data; // Store a pointer to the t_data structure
		threadData[0].threadIndex = 0;
		render(&threadData[0]);
	}
}

// -----------------------------------------------------------------------------

void init_scene(t_scene *s)
{
	init_camera(&s->camera);
	init_scene_one(s);
	return ;
}

int32_t main(int32_t argc, const char* argv[])
{
	mlx_t* mlx;

	t_data	data;
	// Gotta error check this stuff
	int width = WIDTH;
	int height = HEIGHT;
	if (PIXEL_SIZE > 1)
	{
		width *= PIXEL_SIZE;
		height *= PIXEL_SIZE;
	}
	data.mlx = mlx_init(width, height, "Ray Tracer", false);
	if (data.mlx == NULL)
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	image = mlx_new_image(data.mlx, width, height);
	if (image == NULL)
	{
		mlx_close_window(data.mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (mlx_image_to_window(data.mlx, image, 0, 0) == -1)
	{
		mlx_close_window(data.mlx);
		exit(EXIT_FAILURE);
	}
	// if (!(data.mlx = mlx_init(width, height, "MLX42", true)))
	// {
	// 	puts(mlx_strerror(mlx_errno));
	// 	return(EXIT_FAILURE);
	// }
	// if (!(image = mlx_new_image(data.mlx, width, height)))
	// {
	// 	mlx_close_window(data.mlx);
	// 	puts(mlx_strerror(mlx_errno));
	// 	return(EXIT_FAILURE);
	// }
	// if (mlx_image_to_window(data.mlx, image, 0, 0) == -1)
	// {
	// 	mlx_close_window(data.mlx);
	// 	puts(mlx_strerror(mlx_errno));
	// 	return(EXIT_FAILURE);
	// }

	init_scene(&data.scene);

	recalculate_view(&data);
	recalculated_projection(&data);
	recalculat_ray_directions(&data);
	
	// mlx_loop_hook(data.mlx, ft_randomize, data.mlx);

	mlx_loop_hook(data.mlx, render_loop, &data);

	mlx_loop_hook(data.mlx, ft_hook, data.mlx);
	// mlx_loop_hook(data.mlx, ft_multi_thread, data.mlx);
	
	mlx_loop_hook(data.mlx, frame_times, data.mlx);

	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
	return (EXIT_SUCCESS);
}


// int	main(void)
// {
// 	float	*num = NULL;

// 	array_push(num, 1);
// 	array_push(num, 2);
// 	array_push(num, 3);
// 	array_push(num, 4);
// 	array_push(num, 5);
// 	array_push(num, 6);
// 	array_push(num, 7);

// 	int	length = array_length(num);

// 	for (int i = 0; i < length; ++i)
// 	{
// 		printf("num = %f\n", num[i]);
// 	}

// 	// printf("num = %d\n", num[0]);
// }