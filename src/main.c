/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/09/11 13:18:28 by imisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
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


int next = 0;

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
	return (resuls / FLT_MAX);
}

bool	sphere_intersect(t_ray ray, t_vec3 center, float radius)
{
	t_vec3 oc = vec3_sub(ray.origin, center);
	float a = vec3_dot(ray.direction, ray.direction);
	float b = 2.0 * vec3_dot(oc, ray.direction);
	float c = vec3_dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

void ft_randomize(void* param)
{
	uint32_t color;
	// for (int y = 0; y < HEIGHT; y++)
	for (int y = HEIGHT - 1; y >= 0; y--)
	{
		// usleep(5000);
		for (int x = 0; x < WIDTH; x++)
		{
			t_vec2 coord = {(float)x / (float)(WIDTH), (float)y / (float)(HEIGHT)};
			// coord = vec2_subf(vec2_mulf(coord, 2.0), 1.0f);

			t_vec2 numPixels = vec2_new((float)WIDTH, (float)HEIGHT);
			// t_vec2 numPixels = vec2_new((float)x, (float)y);
			t_vec2 pixelCoord = vec2_mul(coord, numPixels);
			uint32_t pixelIndex = pixelCoord.x + pixelCoord.y * numPixels.x;
			// uint32_t rngState = pixelIndex + total_frames * 719393;
			
			float c = (float)pixelIndex / (float)(WIDTH * HEIGHT);
			// c = c * 2.0f - 1.0f;
			uint32_t color = vec4_to_color(vec4_new(c, c, c, 1.0f));


			// uint32_t rngState = pixelIndex;
			// float	r = random_value(&rngState);
			// float	g = random_value(&rngState);
			// float	b = random_value(&rngState);
			// color = vec4_to_color(vec4_new(r, g, b, 1.0f));


			// t_ray ray;
			// ray.origin = vec3_new(0.0f, 0.0f, -2.0f);
			// ray.direction = vec3_new(coord.x, coord.y, 1.0f);
			// if (sphere_intersect(ray, vec3_new(0.0f, 0.0f, 1.0f), 0.5f))
			// 	color = vec4_to_color(vec4_new(1.0f, 0.0f, 0.0f, 1.0f));
			// else
			// 	color = vec4_to_color(vec4_new(0.0f, 0.0f, 1.0f, 1.0f));

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

void	ft_loop_hook(void *param)
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

	// sky(data);
	total_frames++;
}

// -----------------------------------------------------------------------------

int32_t main(int32_t argc, const char* argv[])
{
	mlx_t* mlx;

	// Gotta error check this stuff
	int width = WIDTH;
	int height = HEIGHT;
	if (PIXEL_SIZE > 1)
	{
		width *= PIXEL_SIZE;
		height *= PIXEL_SIZE;
	}
	mlx = mlx_init(width, height, "Ray Tracer", false);
	if (mlx == NULL)
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	image = mlx_new_image(mlx, width, height);
	if (image == NULL)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (mlx_image_to_window(mlx, image, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		exit(EXIT_FAILURE);
	}
	// if (!(mlx = mlx_init(width, height, "MLX42", true)))
	// {
	// 	puts(mlx_strerror(mlx_errno));
	// 	return(EXIT_FAILURE);
	// }
	// if (!(image = mlx_new_image(mlx, width, height)))
	// {
	// 	mlx_close_window(mlx);
	// 	puts(mlx_strerror(mlx_errno));
	// 	return(EXIT_FAILURE);
	// }
	// if (mlx_image_to_window(mlx, image, 0, 0) == -1)
	// {
	// 	mlx_close_window(mlx);
	// 	puts(mlx_strerror(mlx_errno));
	// 	return(EXIT_FAILURE);
	// }
	
	mlx_loop_hook(mlx, ft_randomize, mlx);
	mlx_loop_hook(mlx, ft_hook, mlx);
	mlx_loop_hook(mlx, ft_loop_hook, mlx);

	mlx_loop(mlx);
	mlx_terminate(mlx);
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