/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/09/09 15:42:17 by imisumi-wsl      ###   ########.fr       */
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
int next = 0;

void ft_randomize(void* param)
{
	uint32_t color;
	for (int y = HEIGHT - 1; y >= 0; y--)
	{
		// usleep(5000);
		for (int x = 0; x < WIDTH; x++)
		{
			color = ft_pixel(
						rand() % 0xFF, // R
						rand() % 0xFF, // G
						rand() % 0xFF, // B
						rand() % 0xFF  // A
					);
				


			t_vec2 coord = {(float)x / (float)(WIDTH), (float)y / (float)(HEIGHT)};
			coord = vec2_subf(vec2_mulf(coord, 2.0), 1.0f);

			t_vec2 numPixels = vec2_new((float)WIDTH, (float)HEIGHT);
			t_vec2 pixelCoord = vec2_mul(coord, numPixels);
			uint32_t pixelIndex = (pixelCoord.x * pixelCoord.y) + numPixels.x;
			// uint32_t rngState = pixelIndex + total_frames * 719393;
			
			for (int i = 0; i < PIXEL_SIZE; i++)
			{
				for (int j = 0; j < PIXEL_SIZE; j++)
				{
					// usleep(5);
					// put_pixel(d->img, (x * PIXEL_SIZE) + i, \
					// 	((HEIGHT - y - 1) * PIXEL_SIZE) + j, \
					// 	vec4_to_color(accumulated_color));
					// mlx_put_pixel(image, (x * PIXEL_SIZE) + i, \
					// 	((HEIGHT - y - 1) * PIXEL_SIZE) + j, \
					// 	color);
					mlx_put_pixel(image, (x * PIXEL_SIZE) + i, \
						((HEIGHT - y - 1) * PIXEL_SIZE) + j, \
						pixelIndex);
				}
			}
		}
		// exit(0);
	}

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
	mlx = mlx_init(width, height, "Ray Tracer", true);
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