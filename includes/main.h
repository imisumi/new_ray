/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichiro <ichiro@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/12/05 00:34:11 by ichiro           ###   ########.fr       */
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
# include "tinyexr.h"

# define WIDTH 500
# define HEIGHT 500
// # define WIDTH 2048
// # define HEIGHT 1024
# define Y_MIN 120
# define Y_MAX 200

# define PIXEL_SIZE 2
# define MT 0
# define THREADS 8
# define ANTIALIASING 1

# define MAX_BOUNCHES 8

# define _USE_MATH_DEFINES

# define MAX_TRIS_LEAF 4

# define RENDER_BVH 0

# define PI 3.14159265359
# define HALF_PI 1.57079632679
# define TWO_PI 6.28318530718

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
	t_vec3	color;

	float	roughness;
	float	specular;
	t_vec3	specular_color;
	t_vec3	emission_color;
	float	emission_strength;

	// float	refractive_index;
}	t_material;

typedef struct s_sphere
{
	t_vec3		position;
	float		radius;
	uint8_t	material_index;
	t_material	material;
	bool	color;
	t_vec3	col;
}	t_sphere;

typedef struct s_plane
{
	t_vec3		position;
	t_vec3		normal;
	float		width;
	float		height;
	t_material	material;
	uint8_t		material_index;
}	t_plane;

typedef struct s_aabb
{
	t_vec3	min;
	t_vec3	max;
}	t_aabb;

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
	t_vec4	*ray_target;
}	t_camera;

typedef struct s_ambient
{
	bool	enabled;
	t_vec3	color;
	float	strength;
}	t_ambient;

typedef struct s_scene
{
	t_ambient	ambient;

	t_sphere	*spheres;
	t_plane		*planes;
	t_material	*materials;
	t_camera	camera;
}				t_scene;

typedef struct s_utils
{
	pthread_t			threads[THREADS];
	t_render_block		blocks[THREADS];
	pthread_mutex_t		mutex;
	uint32_t			accumulated_frames;
	t_vec4				*accumulated_data;
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

typedef struct s_vert
{
	t_vec3	position;
	t_vec3	normal;
	t_vec2	uv;
}	t_vert;

typedef struct s_tri_faces
{
	t_vec3	index[3];
}	t_tri_faces;

typedef struct s_face
{
	int	index[3];
	int	normal_index[3];
}	t_face;

typedef struct s_tri
{
	t_vec3	a;
	t_vec3	b;
	t_vec3	c;
	t_vec3	normal;
} t_tri;

typedef struct s_bvh_node
{
	int	start;
	int	end;
	t_aabb aabb;
	struct s_bvh_node *left;
	struct s_bvh_node *right;
	bool is_leaf;
}	t_bvh_node;

typedef struct s_light
{
	t_vec3	position;
	t_vec3	color;
	float	strength;
}	t_light;

typedef struct s_hitinfo
{
	bool	hit;
	float	distance;
	t_vec3	position;
	t_vec3	normal;

	t_material	material;
	
}	t_hitinfo;


// t_vec4 *accumulated_data;


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

void anti_aliasing(t_data *d);


// intersection.c

// t_hitinfo	sphere_intersection(t_ray ray, t_sphere sphere);
// t_hitinfo	sphere_intersection(t_ray ray, t_sphere *spheres);
// t_hitinfo	sphere_intersection(t_ray ray, t_sphere *spheres, t_hitinfo hitinfo);
t_hitinfo		sphere_intersection(t_ray ray, t_scene s, t_hitinfo hitinfo);
// t_hitinfo	plane_intersection(t_ray ray, t_plane *planes, t_hitinfo obj_hit);
t_hitinfo	plane_intersection(t_ray ray, t_scene s, t_hitinfo obj_hit);

// scene.c
t_sphere	create_sphere(t_vec3 center, float radius);
void		init_scene_one(t_scene *scene);
void		init_scene_two(t_scene *scene);
void		init_scene_three(t_scene *scene);
void		init_scene_four(t_scene *scene);





t_vert	*vert_cube();
t_tri_faces	*cube_faces();
t_hitinfo triangle_intersection(t_ray ray, t_hitinfo obj_hit, t_tri tri);


// void	load_obj_file_data(char *filename, t_vec3 **vertex, t_face **faces, t_vec3 **vn);
void	load_obj_file_data(char *filename, t_vec3 **vertex, t_face **faces, t_vec3 **vn, t_vec3 **normal_index);
t_hitinfo intersectRayPlane(t_ray ray, float yLevel, t_hitinfo hitinfo);








bool	parse_input(int argc, char **argv, t_data *data);
float	string_to_float(char *str);
bool	srgb_to_vec3(char *str, t_vec3 *color);
int		char_count(char *str, char c);
int		ft_2d_strlen(char **arr);
void	free_2d_arr(char **arr);

bool	parse_get_pos(char *str, t_vec3 *pos);

bool	parse_get_normal(char *str, t_vec3 *normal);

t_sphere	new_sphere(t_vec3 pos, float radius, t_vec3 color);
uint8_t		parse_get_material_index(t_data *data, char *str);