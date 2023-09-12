/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/09/12 23:43:42 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <GLFW/glfw3.h>
# include <float.h>
# include <math.h>
# include <pthread.h>

# include "MLX42.h"
# include "libft.h"
# include "darray.h"
# include "lib3d.h"

#define WIDTH 1050
#define HEIGHT 1050
#define PIXEL_SIZE 1
#define MT 1
#define THREADS 14

#define _USE_MATH_DEFINES

typedef struct s_render_block
{
	int	x_start;
	int	x_end;
	int	y_start;
	int	y_end;
}	t_render_block;

typedef struct {
	int	x_start;
	int	x_end;
	int	y_start;
	int	y_end;
}	RenderBlock;

typedef struct s_material
{
	t_vec4	color;
}	t_material;

typedef struct s_sphere
{
	t_vec3		position;
	float		radius;
	t_material	material;
}	t_sphere;


typedef struct s_camera
{
	t_mat4	projection;
	t_mat4	inv_projection;
	t_mat4	view;
	t_mat4	inv_view;

	float	vertical_fov;
	float	aspectRatio;
	float	zNear;
	float	zFar;

	t_vec3	position;
	t_vec3	direction;

	t_vec2	mouse_delta;
	t_vec2	mouse_pos;
	bool	mouse_lock;
	t_vec2	prev_mouse_pos;

	t_vec3	*ray_dir;
}	t_camera;

typedef struct s_scene
{
	t_sphere	*spheres;
	t_camera	camera;
}				t_scene;

typedef struct s_utils
{
	pthread_t			threads[THREADS];
	t_render_block		blocks[THREADS];
}				t_utils;

typedef struct s_data
{
	t_utils			utils;
	t_scene			scene;
	mlx_t			*mlx;
	mlx_image_t		*img;
}				t_data;

typedef struct
{
	t_data	*data;
	int threadIndex;
} t_thread_data;

typedef struct s_hitinfo
{
	bool	hit;
	float	distance;
	t_vec3	position;
	t_vec3	normal;

	t_material	material;
}	t_hitinfo;





// t_hitinfo	sphere_intersection(t_ray ray, t_vec3 center, float radius);

// #define WIDTH 1600
// #define HEIGHT 1600
// #define PIXEL_SIZE 1


// float4 frag (v2f i) : SV_Target
// {
// 	uint2 numpixles = _ScreenParams.xy;
// 	uint2 pixelcoord = i.uv * numpixles;
// 	uint pixelindex = pixelcoord.x + pixelcoord.y * numpixles.x;

// 	return pixelindex / (float)(numpixels.x * numpixels.y);
// }

// void	*change_num(int *x)
// {
// 	*x++;
// }

// void	*thread()
// {
// 	int x = 0;

// 	change_num(&x);
// }

// main()
// {
// 	for (int i = 0; i < 8; i++)
// 		thread();
// }





// camera.c
void	recalculat_ray_directions(t_data *d);
void	init_camera(t_camera *cam);
void	recalculate_view(t_data *d);
void	recalculated_projection(t_data *d);
void	movement(t_data *d);


// intersection.c

t_hitinfo	sphere_intersection(t_ray ray, t_sphere sphere);

// scene.c
t_sphere	create_sphere(t_vec3 center, float radius);
void		init_scene_one(t_scene *scene);