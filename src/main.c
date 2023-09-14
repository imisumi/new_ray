/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/09/14 20:22:24 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"
// #include "main.h"
#include <float.h>
#include <pthread.h>
static mlx_image_t* image;
int	total_frames = 0;
double previousTime = 0.0;
// uint32_t accumulated_frames = 1;
// -----------------------------------------------------------------------------

float my_sign(float num) {
    if (num > 0.0f) {
        return 1.0f;
    } else if (num < 0.0f) {
        return -1.0f;
    } else {
        return 0.0f;
    }
}


float randomFloat(uint32_t *state)
{
	*state = *state * 747796405 + 2891336453;
	uint32_t result = ((*state >> ((*state >> 28) + 4)) ^ *state) * 277803737;
	result = (result >> 22) ^ result;
	return (float)result / 4294967295.0f;
}

float random_value_normal_distribution(uint32_t *state)
{
	float theta = 2 * 3.1415926 * randomFloat(state);
	float rho = sqrtf(-2 * logf(randomFloat(state)));
	return rho * cosf(theta);
}

t_vec3 random_direction(uint32_t *state)
{
	float x = random_value_normal_distribution(state);
	float y = random_value_normal_distribution(state);
	float z = random_value_normal_distribution(state);
	return vec3_normalize(vec3_new(x, y, z));
	
}

t_vec3 random_himisphere_dir(t_vec3 normal, uint32_t *state)
{

	t_vec3 dir;
	dir = random_direction(state);

	return vec3_mulf(dir, my_sign(vec3_dot(normal, dir)));
}


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
	t_data *data = arg;
	data->utils.accumulated_frames++;
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

t_vec3 vec3_reflect(t_vec3 incident, t_vec3 normal)
{
	return (vec3_sub(incident, vec3_mulf(normal, 2.0f * vec3_dot(incident, normal))));
}

t_vec3 lerp(t_vec3 vec1, t_vec3 vec2, float t)
{
	// if (t == 1.0f)
	// 	return vec1;
	t_vec3 result;
	result.x = vec1.x + (t * (vec2.x - vec1.x));
	result.y = vec1.y + (t * (vec2.y - vec1.y));
	result.z = vec1.z + (t * (vec2.z - vec1.z));
	return result;
}

t_vec4	per_pixel(t_ray ray, t_scene s, t_vec2 xy, uint32_t *rngState, t_vec2 coord)
{
	int	num_spheres = 1;
	int	i = 0;
	float *f;
	int	bounces = 0;
	int	max_bounces = 5;

	t_hitinfo closest_hit;
	t_vec3	incomming_light = vec3_new(0.0f, 0.0f, 0.0f);
	t_vec3 ray_color = vec3_new(1.0f, 1.0f, 1.0f);
	float is_specular = 0.0f;
	while (bounces <= max_bounces)
	{
		closest_hit.hit = false;
		closest_hit.distance = FLT_MAX;
		closest_hit = sphere_intersection(ray, s.spheres, closest_hit);
		closest_hit = plane_intersection(ray, s.planes, closest_hit);

		
		if (closest_hit.hit)
		{
			ray.origin = vec3_add(closest_hit.position, vec3_mulf(closest_hit.normal, 0.001f));
			
			// ray.direction = vec3_normalize(vec3_add(closest_hit.normal, random_direction(rngState)));
			t_vec3 diffuse_dir = vec3_normalize(vec3_add(closest_hit.normal, random_direction(rngState)));
			t_vec3 specular_dir = vec3_reflect(ray.direction, closest_hit.normal);
			// if (closest_hit.material.specular >= randomFloat(rngState))
			// 	is_specular = 1.0f;
			// else
			// 	is_specular = 0.0f;
			is_specular = closest_hit.material.specular >= randomFloat(rngState);
			
			// ray.direction =  lerp(diffuse_dir, specular_dir, \
			// 						closest_hit.material.roughness * closest_hit.material.roughness);
			ray.direction =  lerp(diffuse_dir, specular_dir, \
									closest_hit.material.roughness * is_specular);
			
			t_material material = closest_hit.material;
			t_vec3 emitted_light = vec3_mulf(material.emission_color, material.emission_strength);
			// float	light_strength = vec3_dot(ray.direction, closest_hit.normal);
			incomming_light = vec3_add(incomming_light, vec3_mul(ray_color, emitted_light));
			// ray_color = vec3_mul(ray_color, vec3_mulf(material.color, light_strength * 2.0f));
			ray_color = vec3_mul(ray_color, lerp(material.color, material.specular_color, is_specular));
			// ray_color = vec3_mul(ray_color, material.color);
		}
		else
		{
			t_vec3 unit_direction = vec3_normalize(ray.direction);
			float t = 0.5f * (unit_direction.y + 1.0f);
			t_vec3 sky = vec3_add(vec3_mulf(vec3_new(1.0f, 1.0f, 1.0f), 1.0f - t), vec3_mulf(vec3_new(0.5f, 0.7f, 1.0f), t));
			incomming_light = vec3_add(incomming_light, vec3_mul(ray_color, sky));
			break ;
		}
		bounces++;
	}
	// printf("check\n");
	
	t_vec4 c;
	c.x = incomming_light.x;
	c.y = incomming_light.y;
	c.z = incomming_light.z;
	c.w = 1.0f;
	return (c);
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
	if (data->utils.accumulated_frames == 1)
		memset(data->utils.accumulated_data, 0, WIDTH * HEIGHT * sizeof(t_vec4));
	for (int y = data->utils.blocks[index].y_start; y <data->utils.blocks[index].y_end; y++)
	{
		for (int x = data->utils.blocks[index].x_start; x < data->utils.blocks[index].x_end; x++)
		{
			t_vec2 coord = {(float)x / (float)(WIDTH), (float)y / (float)(HEIGHT)};
			t_vec2 numPixels = vec2_new((float)WIDTH, (float)HEIGHT);
			t_vec2 pixelCoord = vec2_mul(coord, numPixels);
			uint32_t pixelIndex = pixelCoord.x + pixelCoord.y * numPixels.x;
			rngState = pixelIndex + total_frames * 719393;

			ray.direction = data->scene.camera.ray_dir[x + y * WIDTH];
			//! anti-aliasing

			t_vec4 col = per_pixel(ray, data->scene, vec2_new(x, y), &rngState, coord);

			data->utils.accumulated_data[x + y * WIDTH] = vec4_add(data->utils.accumulated_data[x + y * WIDTH], col);
			t_vec4 accumulated_color = data->utils.accumulated_data[x + y * WIDTH];
			accumulated_color = vec4_divf(accumulated_color, data->utils.accumulated_frames);
			accumulated_color = vec4_clamp(accumulated_color, 0.0, 1.0);
			
			color = vec4_to_color(accumulated_color);
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
		// printf("check %d\n", y);
		// exit(0);
	}
	// exit(0);
	// printf("check\n");
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
	if (ANTIALIASING)
		recalculat_ray_directions(data);
	// anti_aliasing
}

// -----------------------------------------------------------------------------

void init_scene(t_scene *s)
{
	init_camera(&s->camera);
	// init_scene_one(s);
	// init_scene_two(s);
	init_scene_three(s);
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
	data.utils.accumulated_frames = 1;
	data.utils.accumulated_data = malloc(sizeof(t_vec4) * WIDTH * HEIGHT);
	data.scene.camera.ray_target = malloc(sizeof(t_vec4) * WIDTH * HEIGHT);

	init_scene(&data.scene);

	recalculate_view(&data);
	recalculated_projection(&data);
	recalculat_ray_directions(&data);
	
	// mlx_loop_hook(data.mlx, ft_randomize, data.mlx);

	mlx_loop_hook(data.mlx, render_loop, &data);

	mlx_loop_hook(data.mlx, ft_hook, data.mlx);
	// mlx_loop_hook(data.mlx, ft_multi_thread, data.mlx);
	
	mlx_loop_hook(data.mlx, frame_times, &data);

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