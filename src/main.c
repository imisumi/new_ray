/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/10/27 14:38:31 by imisumi          ###   ########.fr       */
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

t_vec3	*vertex;
t_face	*faces;
t_tri	*tris;
t_vec3	*vn;
t_vec3 	*normal_index;
t_aabb bvh;
t_bvh_node *bvh_nodes;
// -----------------------------------------------------------------------------

float	delta_time()
{
	
}

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
	float random_float = randomFloat(state);
	//! avoid NaN
	while (random_float <= FLT_EPSILON)
		random_float = randomFloat(state);
	// float rho = sqrtf(-2 * logf(randomFloat(state)));
	float rho = sqrtf(-2 * logf(random_float));
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


void	reverseString(char str[], int length)
{
	int		start;
	int		end;
	char	temp;

	start = 0;
	end = length - 1;
	while (start < end)
	{
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

char	*floatToString(float num, char* str, int precision)
{
	int wholePart = (int)num;
	// Handle negative numbers
	if (wholePart < 0)
	{
		str[0] = '-';
		str++;
		wholePart = -wholePart;
	}
	// Convert the whole part to string
	int i = 0;
	while (wholePart != 0)
	{
		str[i++] = (wholePart % 10) + '0';
		wholePart /= 10;
	}
	if (i == 0)
		str[i++] = '0';
	reverseString(str, i);
	// Handle precision (decimal part)
	if (precision > 0)
	{
		str[i++] = '.';
		num -= (int)num; // Get the decimal part
		for (int j = 0; j < precision; j++)
		{
			num *= 10;
			int digit = (int)num;
			str[i++] = digit + '0';
			num -= digit;
		}
	}
	str[i] = '\0'; // Null-terminate the string
	return str;
}

void	frame_times(void *arg)
{
	t_data *data = arg;
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - previousTime;
	double frameTimeMs = deltaTime * 1000.0;
	double fps = 1000.0 / frameTimeMs;
	//! use this one
	// printf("\rframeTimeMs: %.2f ms    frames: %d	accumulated frames: %d", frameTimeMs, total_frames, accumulated_frames);
	printf("\rframeTimeMs: %.2f ms    frames: %d", frameTimeMs, total_frames);
	char output[50];

	// snprintf(output, 50, "%f", fps);
	floatToString(fps, output, 2);
	mlx_set_window_title(data->mlx, output);


	
	// printf("\rFPS: %.2f    ",1000 / frameTimeMs);
	fflush(stdout);
	previousTime = currentTime;

	total_frames++;
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
		// printf("MT = 0\n");
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

// bool intersectAABB(t_ray ray, t_aabb box, t_hitinfo *obj_hit)
// {
// 	float tMinX = (box.min.x - ray.origin.x) / ray.direction.x;
// 	float tMaxX = (box.max.x - ray.origin.x) / ray.direction.x;

// 	float tMinY = (box.min.y - ray.origin.y) / ray.direction.y;
// 	float tMaxY = (box.max.y - ray.origin.y) / ray.direction.y;

// 	float tMinZ = (box.min.z - ray.origin.z) / ray.direction.z;
// 	float tMaxZ = (box.max.z - ray.origin.z) / ray.direction.z;

// 	float tMin = fmaxf(fmaxf(fminf(tMinX, tMaxX), fminf(tMinY, tMaxY)), fminf(tMinZ, tMaxZ));
// 	float tMax = fminf(fminf(fmaxf(tMinX, tMaxX), fmaxf(tMinY, tMaxY)), fmaxf(tMinZ, tMaxZ));

// 	// if (tMax < 0)
// 	// 	return false;
// 	// return tMin <= tMax;
// 	if (tMax < 0 || tMin > tMax)
// 		return false; // No intersection

// 	obj_hit->hit = true;
// 	if (tMin == tMinX) {
// 		obj_hit->normal = (ray.origin.x < box.min.x) ? (t_vec3){-1, 0, 0} : (t_vec3){1, 0, 0};
// 	} else if (tMin == tMinY) {
// 		obj_hit->normal = (ray.origin.y < box.min.y) ? (t_vec3){0, -1, 0} : (t_vec3){0, 1, 0};
// 	} else {
// 		obj_hit->normal = (ray.origin.z < box.min.z) ? (t_vec3){0, 0, -1} : (t_vec3){0, 0, 1};
// 	}
// 	return true;
// }

t_vec3 vec3_negate(t_vec3 v) {
    t_vec3 result;
    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;
    return result;
}

bool intersectAABB(t_ray ray, t_aabb box, t_hitinfo *obj_hit)
{
    float tMinX = (box.min.x - ray.origin.x) / ray.direction.x;
    float tMaxX = (box.max.x - ray.origin.x) / ray.direction.x;

    float tMinY = (box.min.y - ray.origin.y) / ray.direction.y;
    float tMaxY = (box.max.y - ray.origin.y) / ray.direction.y;

    float tMinZ = (box.min.z - ray.origin.z) / ray.direction.z;
    float tMaxZ = (box.max.z - ray.origin.z) / ray.direction.z;

    float tMin = fmaxf(fmaxf(fminf(tMinX, tMaxX), fminf(tMinY, tMaxY)), fminf(tMinZ, tMaxZ));
    float tMax = fminf(fminf(fmaxf(tMinX, tMaxX), fmaxf(tMinY, tMaxY)), fmaxf(tMinZ, tMaxZ));

    if (tMax < 0 || tMin > tMax) {
        // No intersection
        obj_hit->hit = false;
        return false;
    }

    // Intersection occurred, calculate hit normal based on which axis tMin belongs to
    if (tMin == tMinX) {
        obj_hit->normal = (ray.origin.x < box.min.x) ? vec3_new(-1, 0, 0) : vec3_new(1, 0, 0);
    } else if (tMin == tMinY) {
        obj_hit->normal = (ray.origin.y < box.min.y) ? vec3_new(0, -1, 0) : vec3_new(0, 1, 0);
    } else {
        obj_hit->normal = (ray.origin.z < box.min.z) ? vec3_new(0, 0, -1) : vec3_new(0, 0, 1);
    }

	 if (vec3_dot(obj_hit->normal, ray.direction) > 0) {
        // Flip the normal if it's facing the wrong direction
        obj_hit->normal = vec3_negate(obj_hit->normal);
    }


    obj_hit->hit = true;
    return true;
}

bool	vec3_cmp(t_vec3 vec1, t_vec3 vec2)
{
	if (vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z)
		return true;
	return false;
}

t_vec3	vec3_up(void)
{
	return (vec3_new(0.0f, 1.0f, 0.0f));
}

t_vec3	vec3_down(void)
{
	return (vec3_new(0.0f, -1.0f, 0.0f));
}

t_vec3	vec3_left(void)
{
	return (vec3_new(-1.0f, 0.0f, 0.0f));
}

t_vec3	vec3_right(void)
{
	return (vec3_new(1.0f, 0.0f, 0.0f));
}

t_vec3	vec3_forward(void)
{
	return (vec3_new(0.0f, 0.0f, 1.0f));
}

t_vec3	vec3_backward(void)
{
	return (vec3_new(0.0f, 0.0f, -1.0f));
}

//  return (pow(sun, 256.0) + 0.2 * pow(sun, 2.0)) * vec3(2.0, 1.6, 1.0) +
//          pow(ground, 0.5) * vec3(0.7, 0.6, 0.5) +
//          pow(sky, 1.0) * vec3(0.3, 0.5, 0.8);

t_vec3	vec3_addf(t_vec3 vec, float f)
{
	t_vec3 result;
	result.x = vec.x + f;
	result.y = vec.y + f;
	result.z = vec.z + f;
	return result;
}


t_vec3	background(float t, t_vec3 rd)
{
	t_vec3 light = vec3_normalize(vec3_new(sin(t), 0.6f, cos(t)));
	float sun = fmaxf(0.0f, vec3_dot(rd, light));
	float sky = fmaxf(0.0f, vec3_dot(rd, vec3_new(0.0f, 1.0f, 0.0f)));
	float ground = fmaxf(0.0f, -vec3_dot(rd, vec3_new(0.0f, 1.0f, 0.0f)));

	// t_vec3 col = vec3_add(vec3_mulf(vec3_new(2.0f, 1.6f, 1.0f), (powf(sun, 256.0) + 0.2 * powf(sun, 2.0))), \
	// 			vec3_add(vec3_mulf(vec3_new(0.7, 0.6, 0.5), powf(ground, 0.5f)), \
	// 						vec3_mulf(vec3_new(0.3f, 0.5f, 0.8f), powf(sky, 1.0f))));
	t_vec3 col = vec3_add(vec3_addf(vec3_mulf(vec3_mulf(vec3_new(2.0f, 1.6f, 1.0f), powf(sun, 2.0f)), 0.2f), powf(sun, 256.0f)), \
				vec3_add(vec3_mulf(vec3_new(0.7, 0.6, 0.5), powf(ground, 0.5f)), \
							vec3_mulf(vec3_new(0.3f, 0.5f, 0.8f), powf(sky, 1.0f))));
	// vec3_mulf(vec3_new(0.3f, 0.5f, 0.8f), powf(ground, 1.0f));
	// vec3_mulf(vec3_new(0.3f, 0.5f, 0.8f), powf(sky, 1.0f));
	return col;
}

t_vec3 vec3_min(t_vec3 a, t_vec3 b) {
    t_vec3 result;
    result.x = fminf(a.x, b.x);
    result.y = fminf(a.y, b.y);
    result.z = fminf(a.z, b.z);
    return result;
}

t_vec3 vec3_max(t_vec3 a, t_vec3 b) {
    t_vec3 result;
    result.x = fmaxf(a.x, b.x);
    result.y = fmaxf(a.y, b.y);
    result.z = fmaxf(a.z, b.z);
    return result;
}

t_vec3 box(t_vec3 ray_origin, t_vec3 ray_dir, t_vec3 minpos, t_vec3 maxpos) {
	// t_vec3 inverse_dir = 1.0 / ray_dir;
	t_vec3 inverse_dir;
	inverse_dir.x = 1.0f / ray_dir.x;
	inverse_dir.y = 1.0f / ray_dir.y;
	inverse_dir.z = 1.0f / ray_dir.z;
	// t_vec3 tbot = inverse_dir * (minpos - ray_origin);
	t_vec3 tbot = vec3_mul(inverse_dir, vec3_sub(minpos, ray_origin));
	// t_vec3 ttop = inverse_dir * (maxpos - ray_origin);
	t_vec3 ttop = vec3_mul(inverse_dir, vec3_sub(maxpos, ray_origin));
	t_vec3 tmin = vec3_min(ttop, tbot);
	t_vec3 tmax = vec3_max(ttop, tbot);
	// t_vec2 traverse = max(tmin.xx, tmin.yz);
	t_vec2 traverse;
	traverse.x = fmaxf(tmin.x, tmin.y);
	traverse.y = fmaxf(tmin.z, tmin.y);
	float traverselow = fmaxf(traverse.x, traverse.y);
	// traverse = minf(tmax.xx, tmax.yz);
	traverse.x = fminf(tmax.x, tmax.y);
	traverse.y = fminf(tmax.z, tmax.y);
	float traversehi = fminf(traverse.x, traverse.y);
	return vec3_new(traversehi > fmaxf(traverselow, 0.0), traversehi, traverselow);
}

t_vec3	vec3_clampf(t_vec3 vec, float min, float max)
{
	t_vec3 result;
	result.x = fminf(fmaxf(vec.x, min), max);
	result.y = fminf(fmaxf(vec.y, min), max);
	result.z = fminf(fmaxf(vec.z, min), max);
	return result;
}

t_vec3	vec3_floorf(t_vec3 vec)
{
	t_vec3 result;
	result.x = floorf(vec.x);
	result.y = floorf(vec.y);
	result.z = floorf(vec.z);
	return result;
}

t_vec3	vec3_mix(t_vec3 vec1, t_vec3 vec2, float t)
{
	t_vec3 result;
	result.x = vec1.x + (t * (vec2.x - vec1.x));
	result.y = vec1.y + (t * (vec2.y - vec1.y));
	result.z = vec1.z + (t * (vec2.z - vec1.z));
	return result;
}

t_vec3 calculate_sky_color(t_vec3 ray_direction) {
    // Normalize the ray direction
    float length = sqrt(ray_direction.x * ray_direction.x + ray_direction.y * ray_direction.y + ray_direction.z * ray_direction.z);
    ray_direction.x /= length;
    ray_direction.y /= length;
    ray_direction.z /= length;

    // Calculate sky color based on ray direction (realistic gradient example)
    // In this example, the color changes from dark blue at the zenith to light blue at the horizon
    // You can modify this gradient to create different sky effects
    float t = 0.5f * (ray_direction.y + 1.0f); // Map y component to [0, 1]
    t_vec3 sky_color;
    sky_color.x = 0.5f + t * 0.5f; // Red component
    sky_color.y = 0.6f + t * 0.4f; // Green component
    sky_color.z = 1.0f; // Blue component

    return sky_color;
}

// t_vec3 calculate_lighting(t_vec3 surface_normal, t_vec3 light_direction) {
//     // Lambertian reflection model: diffuse reflection
//     float diffuse_intensity = fmaxf(0.0f, vec3_dot(surface_normal, light_direction));
//     t_vec3 light_color = vec3_new(1.0f, 1.0f, 1.0f);  // White light color
//     t_vec3 diffuse_component = vec3_mulf(light_color, diffuse_intensity);
//     return diffuse_component;
// }

// t_vec3 calculate_shading(t_vec3 surface_normal) {
//     // Lambertian reflection model: diffuse reflection
//     float diffuse_intensity = fmaxf(0.0f, vec3_dot(surface_normal, vec3_new(1.0f, 1.0f, 1.0f))); // Assuming light from down-left-forward direction
//     t_vec3 shading_color = vec3_new(diffuse_intensity, diffuse_intensity, diffuse_intensity);
//     return shading_color;
// }

float intersectCube(t_ray ray, t_vec3 close, t_vec3 far)
{
	float t1,t2,tnear = -1000.0f,tfar = 1000.0f,temp,tCube;
	bool intersectFlag = true;

	float rayDirection[3];
	rayDirection[0] = ray.direction.x;
	rayDirection[1] = ray.direction.y;
	rayDirection[2] = ray.direction.z;

	float rayStart[3];
	rayStart[0] = ray.origin.x;
	rayStart[1] = ray.origin.y;
	rayStart[2] = ray.origin.z;

	float b1[3];
	b1[0] = close.x;
	b1[1] = close.y;
	b1[2] = close.z;

	float b2[3];
	b2[0] = far.x;
	b2[1] = far.y;
	b2[2] = far.z;
	
	for(int i =0 ;i < 3; i++)
	{
		if(rayDirection[i] == 0)
		{
			if(rayStart[i] < b1[i] || rayStart[i] > b2[i])
				intersectFlag = false;
		}
		else
		{
			t1 = (b1[i] - rayStart[i])/rayDirection[i];
			t2 = (b2[i] - rayStart[i])/rayDirection[i];
		if(t1 > t2)
		{
			temp = t1;
			t1 = t2;
			t2 = temp;
		}
		if(t1 > tnear)
			tnear = t1;
		if(t2 < tfar)
			tfar = t2;
		if(tnear > tfar)
			intersectFlag = false;
		if(tfar < 0)
			intersectFlag = false;
		}
	}
	if(intersectFlag == false)
		tCube = 0;
	else
		tCube = tnear;
	
	return tCube;
}

t_hitinfo bvh_rec(t_ray ray, t_scene s, t_vec2 xy, uint32_t *rngState, t_vec2 coord, t_bvh_node *bvh_nodes)
{
	int	i = 0;
	int	bounces = 0;
	t_hitinfo closest_hit;
	t_vec3	incomming_light = vec3_new(0.0f, 0.0f, 0.0f);
	t_vec3 ray_color = vec3_new(1.0f, 1.0f, 1.0f);


	closest_hit.material.color = vec3_new(0.0f, 0.0f, 0.0f);
	
	while (bounces <= MAX_BOUNCHES)
	{
		closest_hit.hit = false;
		closest_hit.distance = FLT_MAX;


		if (bvh_nodes->is_leaf == true)
		{
			if (intersectCube(ray, bvh_nodes->aabb.min, bvh_nodes->aabb.max) > 0.0f)
			{
				float EPSI = 0.0001;
				float t = intersectCube(ray, bvh_nodes->aabb.min, bvh_nodes->aabb.max);
				if (t > 0.0f)
				{
					t_vec3 p = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
					if (fabsf(p.x - bvh_nodes->aabb.min.x) < EPSI) // left
						closest_hit.material.color = vec3_new(0.5, 0.5, 0.5);
					if (fabsf(p.x - bvh_nodes->aabb.max.x) < EPSI) // right
						closest_hit.material.color = vec3_new(0.5, 0.5, 0.5);
					if (fabsf(p.y - bvh_nodes->aabb.min.y) < EPSI) // bottom
						closest_hit.material.color = vec3_new(0.55, 0.55, 0.55);
					if (fabsf(p.y - bvh_nodes->aabb.max.y) < EPSI) // top
						closest_hit.material.color = vec3_new(0.55, 0.55, 0.55);
					if (fabsf(p.z - bvh_nodes->aabb.min.z) < EPSI) // back
						closest_hit.material.color = vec3_new(0.6, 0.6, 0.6);
					if (fabsf(p.z - bvh_nodes->aabb.max.z) < EPSI) // front
						closest_hit.material.color = vec3_new(0.6, 0.6, 0.6);
					closest_hit.material.color = vec3_new(0.4f, 0.4f, 0.4f);
				}
				// return closest_hit;
				for (int i = bvh_nodes->start; i < bvh_nodes->end; i++)
				{
					closest_hit = triangle_intersection(ray, closest_hit, tris[i]);
				}
				if (closest_hit.hit == true)
				{
					closest_hit.material.color = vec3_new(1.0f, 0.0f, 0.0f);
				}
			}
			return closest_hit;
		}
		else
		{
			t_hitinfo left = bvh_rec(ray, s, xy, rngState, coord, bvh_nodes->left);
			t_hitinfo right = bvh_rec(ray, s, xy, rngState, coord, bvh_nodes->right);

			if (left.hit == false && right.hit == false)
				return closest_hit;
			if (left.hit == true && right.hit == false)
				return left;
			if (left.hit == false && right.hit == true)
				return right;
			if (left.distance > right.distance)
				return right;
			else
				return left;
		}

		return closest_hit;
		bounces++;
	}
	return closest_hit;
}

t_hitinfo	render_bvh(t_ray ray, t_hitinfo closest_hit, float t)
{
	t_vec3	color;
	t_vec3	p;
	float	EPSI;

	EPSI = 0.0001;
	p = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
	if (fabsf(p.x - bvh_nodes->aabb.min.x) < EPSI) // left
		closest_hit.material.color = vec3_new(0.5, 0.5, 0.5);
	else if (fabsf(p.x - bvh_nodes->aabb.max.x) < EPSI) // right
		closest_hit.material.color = vec3_new(0.5, 0.5, 0.5);
	else if (fabsf(p.y - bvh_nodes->aabb.min.y) < EPSI) // bottom
		closest_hit.material.color = vec3_new(0.55, 0.55, 0.55);
	else if (fabsf(p.y - bvh_nodes->aabb.max.y) < EPSI) // top
		closest_hit.material.color = vec3_new(0.55, 0.55, 0.55);
	else if (fabsf(p.z - bvh_nodes->aabb.min.z) < EPSI) // back
		closest_hit.material.color = vec3_new(0.6, 0.6, 0.6);
	else if (fabsf(p.z - bvh_nodes->aabb.max.z) < EPSI) // front
		closest_hit.material.color = vec3_new(0.6, 0.6, 0.6);
	closest_hit.hit = true;
	closest_hit.material.color = vec3_new(fabs(p.x), fabs(p.y), fabs(p.z));
	closest_hit.distance = t;
	closest_hit.position = p;
	return (closest_hit);
}

t_hitinfo testing_bvh(t_ray ray, t_bvh_node *bvh_nodes, t_hitinfo closest_hit)
{
	float	t;

	t = intersectCube(ray, bvh_nodes->aabb.min, bvh_nodes->aabb.max);
	if (t)
	{
		if (bvh_nodes->is_leaf == true)
		{
			if (t > 0.0f && t < closest_hit.distance)
			{
				if (RENDER_BVH)
					closest_hit = render_bvh(ray, closest_hit, t);
				else
				{
					for (int i = bvh_nodes->start; i < bvh_nodes->end; i++)
						closest_hit = triangle_intersection(ray, closest_hit, tris[i]);
				}
			}
			return (closest_hit);
		}
		else
		{
			t_hitinfo left = testing_bvh(ray, bvh_nodes->left, closest_hit);
			t_hitinfo right = testing_bvh(ray, bvh_nodes->right, closest_hit);
			if (left.hit && !right.hit)
				return left;
			if (right.hit && !left.hit)
				return right;
			if (left.hit && right.hit)
			{
				if (left.distance < right.distance)
					return left;
				else
					return right;
			}
		}
	}
	return (closest_hit);
}

void	check_nan(t_vec3 vec)
{
	if (isnan(vec.x))
	{
		printf("\nx = nan\n");
		sleep(10000000);
	}
	if (isnan(vec.y))
	{
		printf("\ny = nan\n");
		sleep(10000000);
	}
	if (isnan(vec.z))
	{
		printf("\nz = nan\n");
		sleep(10000000);
	}
}

t_vec4	per_pixel(t_ray ray, t_scene s, t_vec2 xy, uint32_t *rngState, t_vec2 coord)
{
	int	i = 0;
	int	bounces = 0;
	float is_specular = 0.0f;
	t_hitinfo closest_hit;
	t_vec3	incomming_light = vec3_new(0.0f, 0.0f, 0.0f);
	t_vec3 ray_color = vec3_new(1.0f, 1.0f, 1.0f);
	while (bounces <= MAX_BOUNCHES)
	{
		closest_hit.hit = false;
		closest_hit.distance = FLT_MAX;

		closest_hit = testing_bvh(ray, bvh_nodes, closest_hit);

		closest_hit = plane_intersection(ray, s.planes, closest_hit);
		// closest_hit = sphere_intersection(ray, s.spheres, closest_hit);

		if (closest_hit.hit)
		{
			// return vec4_new(closest_hit.material.color.x, closest_hit.material.color.y, closest_hit.material.color.z, 1.0f);

			ray.origin = vec3_add(closest_hit.position, vec3_mulf(closest_hit.normal, 0.00001f));
			
			t_vec3 temp = random_direction(rngState);
			t_vec3 diffuse_dir = vec3_normalize(vec3_add(closest_hit.normal, temp));
			if (isnan(diffuse_dir.x))
			{
				printf("temp = %f %f %f\n", temp.x, temp.y, temp.z);
				printf("closest_hit.normal = %f %f %f\n", closest_hit.normal.x, closest_hit.normal.y, closest_hit.normal.z);
				sleep(10000000);
			}
			t_vec3 specular_dir = vec3_reflect(ray.direction, closest_hit.normal);

			is_specular = closest_hit.material.specular >= randomFloat(rngState);
			
			// check_nan(ray.direction);
			ray.direction =  lerp(diffuse_dir, specular_dir, \
									closest_hit.material.roughness * is_specular);
			if (isnan(ray.direction.x))
			{
				printf("\nx = nan\n");
				printf("diffuse_dir = %f %f %f\n", diffuse_dir.x, diffuse_dir.y, diffuse_dir.z);
				printf("specular_dir = %f %f %f\n", specular_dir.x, specular_dir.y, specular_dir.z);
				printf("roughness = %f\n", closest_hit.material.roughness);
				printf("specular = %f\n", is_specular);
				sleep(10000000);
			}
			check_nan(ray.direction);
			
			t_material material = closest_hit.material;
			t_vec3 emitted_light = vec3_mulf(material.emission_color, material.emission_strength);
			
			incomming_light = vec3_add(incomming_light, vec3_mul(ray_color, emitted_light));
			// check_nan(incomming_light);

			ray_color = vec3_mul(ray_color, lerp(material.color, material.specular_color, is_specular));

			//!russian roullet
			float p = fmaxf(ray_color.x, fmaxf(ray_color.y, ray_color.z));
			if (bounces > 3)
			{
				if (randomFloat(rngState) > p)
					break ;
				ray_color = vec3_divf(ray_color, p);
			}
		}
		else
		{
			// t_vec3 bg = background(0.0f, ray.direction);
			// return vec4_new(bg.x, bg.y, bg.z, 1.0f);
			// check_nan(ray.direction);
			t_vec3 unit_direction = vec3_normalize(ray.direction);
			// check_nan(ray_color);
			// check_nan(unit_direction);
			float t = 0.5f * (unit_direction.y + 1.0f);
			if (t <= 0.0f)
			{
				printf("t < 0.0f\n");
				sleep(5555555);
			}
			t_vec3 sky = vec3_add(vec3_mulf(vec3_new(1.0f, 1.0f, 1.0f), 1.0f - t), vec3_mulf(vec3_new(0.5f, 0.7f, 1.0f), t));
			incomming_light = vec3_add(incomming_light, vec3_mul(ray_color, sky));
			// incomming_light = vec3_add(incomming_light, vec3_new(0.5f, 0.7f, 1.0f));
			break ;
		}

		// return (vec4_new(0.0f, 0.0f, 0.0f, 1.0f));
		bounces++;
	}
	// check_nan(incomming_light);
	return (vec4_new(incomming_light.x, incomming_light.y, incomming_light.z, 1.0f));
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
			if (col.x < 0.0f)
				col.x = 0.0f;
			if (col.y < 0.0f)
				col.y = 0.0f;
			if (col.z < 0.0f)
				col.z = 0.0f;

			// pthread_mutex_lock(&data->utils.mutex);
			data->utils.accumulated_data[x + y * WIDTH] = vec4_add(data->utils.accumulated_data[x + y * WIDTH], col);
			// pthread_mutex_unlock(&data->utils.mutex);
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
		anti_aliasing(data);
}

// -----------------------------------------------------------------------------

t_tri create_tri()
{
	t_tri tri;

	// tri.a = vec3_new(vertex)
}

t_aabb calculate_bounding_box(t_tri *tris, int start, int end)
{
	t_aabb aabb;

	aabb.min = tris[start].a;
	aabb.max = tris[start].a;
	while (start < end)
	{
		if (tris[start].a.x < aabb.min.x)
			aabb.min.x = tris[start].a.x;
		if (tris[start].a.y < aabb.min.y)
			aabb.min.y = tris[start].a.y;
		if (tris[start].a.z < aabb.min.z)
			aabb.min.z = tris[start].a.z;

		if (tris[start].b.x < aabb.min.x)
			aabb.min.x = tris[start].b.x;
		if (tris[start].b.y < aabb.min.y)
			aabb.min.y = tris[start].b.y;
		if (tris[start].b.z < aabb.min.z)
			aabb.min.z = tris[start].b.z;

		if (tris[start].c.x < aabb.min.x)
			aabb.min.x = tris[start].c.x;
		if (tris[start].c.y < aabb.min.y)
			aabb.min.y = tris[start].c.y;
		if (tris[start].c.z < aabb.min.z)
			aabb.min.z = tris[start].c.z;

		//?------------------------------

		if (tris[start].a.x > aabb.max.x)
			aabb.max.x = tris[start].a.x;
		if (tris[start].a.y > aabb.max.y)
			aabb.max.y = tris[start].a.y;
		if (tris[start].a.z > aabb.max.z)
			aabb.max.z = tris[start].a.z;

		if (tris[start].b.x > aabb.max.x)
			aabb.max.x = tris[start].b.x;
		if (tris[start].b.y > aabb.max.y)
			aabb.max.y = tris[start].b.y;
		if (tris[start].b.z > aabb.max.z)
			aabb.max.z = tris[start].b.z;

		if (tris[start].c.x > aabb.max.x)
			aabb.max.x = tris[start].c.x;
		if (tris[start].c.y > aabb.max.y)
			aabb.max.y = tris[start].c.y;
		if (tris[start].c.z > aabb.max.z)
			aabb.max.z = tris[start].c.z;
		
		start++;
	}
	return aabb;
}

t_aabb merge_aabb(t_aabb a, t_aabb b)
{
	t_aabb result;
	result.min.x = fminf(a.min.x, b.min.x);
	result.min.y = fminf(a.min.y, b.min.y);
	result.min.z = fminf(a.min.z, b.min.z);
	result.max.x = fmaxf(a.max.x, b.max.x);
	result.max.y = fmaxf(a.max.y, b.max.y);
	result.max.z = fmaxf(a.max.z, b.max.z);
	return result;
}

t_bvh_node	*build_bvh(t_tri *tris, int start, int end, int max_depth)
{
	t_bvh_node *node = malloc(sizeof(t_bvh_node));
	
	node->start = start;
	node->end = end;
	// node->is_leaf = false;
	if (end - start <= MAX_TRIS_LEAF || max_depth == 0)
	{
		node->is_leaf = true;
		node->aabb = calculate_bounding_box(tris, start, end);
	}
	else
	{
		int mid = (start + end) / 2;
		node->left = build_bvh(tris, start, mid, max_depth - 1);
		// node->right = build_bvh(tris, mid + 1, end, max_depth - 1);
		node->right = build_bvh(tris, mid, end, max_depth - 1);
		
		node->aabb = merge_aabb(node->left->aabb, node->right->aabb);

		node->is_leaf = false;
	}
	return node;
}

t_tri transform_tri(t_tri tri, float x, float y, float z, float s)
{
	tri.a.x = tri.a.x * s + x;
	tri.a.y = tri.a.y * s + y;
	tri.a.z = tri.a.z * s + z;

	tri.b.x = tri.b.x * s + x;
	tri.b.y = tri.b.y * s + y;
	tri.b.z = tri.b.z * s + z;

	tri.c.x = tri.c.x * s + x;
	tri.c.y = tri.c.y * s + y;
	tri.c.z = tri.c.z * s + z;
	return tri;
}

void init_scene(t_scene *s)
{
	vec_init(&tris, 16, sizeof(t_tri));
	vec_init(&vertex, 16, sizeof(t_vec3));
	vec_init(&faces, 16, sizeof(t_face));
	vec_init(&vn, 16, sizeof(t_vec3));
	vec_init(&normal_index, 16, sizeof(t_vec3));
	// load_obj_file_data("cube_tri.obj", &vertex, &faces, &vn, &normal_index);
	// load_obj_file_data("f22.obj", &vertex, &faces, &vn);
	load_obj_file_data("monkey.obj", &vertex, &faces, &vn, &normal_index);
	// load_obj_file_data("sup.obj", &vertex, &faces, &vn);

	// t_face *f;
	// vec_init(&f, 16, sizeof(t_face));
	t_face face;
	face.index[0] = 0;
	face.index[1] = 1;
	face.index[2] = 2;



	for (int i = 0; i < array_length(&faces); i++)
	{
		t_tri tri;
		tri.a = vertex[faces[i].index[0] - 1];
		tri.b = vertex[faces[i].index[1] - 1];
		tri.c = vertex[faces[i].index[2] - 1];
		tri = transform_tri(tri, 0.0, 1.0, 0.0, 0.5);
		tri.normal.x = vn[faces[i].normal_index[0] - 1].x;
		tri.normal.y = vn[faces[i].normal_index[1] - 1].y;
		tri.normal.z = vn[faces[i].normal_index[2] - 1].z;
		array_push(&tris, &tri);
		// printf("normal = %f %f %f\n", tri.normal.x, tri.normal.y, tri.normal.z);
	}

	
	bvh_nodes = build_bvh(tris, 0, array_length(&tris), 50);

	// array_push(&f, &face);
	// array_push(&faces, &face);
	
	// printf("%d\n", array_length(&faces));
	// printf("%d %d %d\n", faces[0].index[0], faces[0].index[1], faces[0].index[2]);

	// printf("%f %f %f\n", tris[0].a.x, tris[0].a.y, tris[0].a.z);
	// exit(0);
	init_camera(&s->camera);
	// init_scene_one(s);
	init_scene_two(s);
	// init_scene_three(s);
	// init_scene_four(s);
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
	data.utils.accumulated_frames = 1;
	data.utils.accumulated_data = malloc(sizeof(t_vec4) * WIDTH * HEIGHT);
	data.scene.camera.ray_target = malloc(sizeof(t_vec4) * WIDTH * HEIGHT);
	mlx_set_window_title(data.mlx, "Testing");

	pthread_mutex_init(&data.utils.mutex, NULL);

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