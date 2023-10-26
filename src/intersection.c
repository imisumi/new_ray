/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/10/26 21:53:16 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"
// # include "lib3d.h"

// t_hitinfo	sphere_intersection(t_ray ray, t_sphere *spheres, t_hitinfo hitinfo)
// {
// 	int	i;

// 	i = 0;
// 	// t_hitinfo	hitinfo = {0};
// 	while (i < array_length(&spheres))
// 	{
// 		// t_hitinfo	hitinfo = {0}; // Initialize all members to zero (null)
	
// 		t_vec3	offset_origin = vec3_sub(ray.origin, spheres[i].position);
	
// 		float	a = vec3_dot(ray.direction, ray.direction);
// 		float	b = 2.0f * vec3_dot(offset_origin, ray.direction);
// 		float	c = vec3_dot(offset_origin, offset_origin) - spheres[i].radius * spheres[i].radius;
	
// 		float	discriminant = b * b - 4 * a * c;
	
// 		if (discriminant >= 0)
// 		{
// 			// printf("discriminant: %f\n", discriminant);
// 			float	t = (-b - sqrtf(discriminant)) / (2.0f * a);
	
// 			if (t >= 0)
// 			{
// 				hitinfo.distance = t;
// 				hitinfo.position = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
// 				hitinfo.normal = vec3_normalize(vec3_sub(hitinfo.position, spheres[i].position));
// 				hitinfo.hit = true;
// 				hitinfo.material = spheres[i].material;
// 				return (hitinfo);
// 			}
// 		}
// 		i++;
// 	}
// 	return (hitinfo);
// }

t_hitinfo	sphere_intersection(t_ray ray, t_sphere *spheres, t_hitinfo hitinfo)
{
	int	i;

	i = 0;
	while (i < array_length(&spheres))
	{
		t_vec3	offset_origin = vec3_sub(ray.origin, spheres[i].position);
	
		float	a = vec3_dot(ray.direction, ray.direction);
		float	b = 2.0f * vec3_dot(offset_origin, ray.direction);
		float	c = vec3_dot(offset_origin, offset_origin) - spheres[i].radius * spheres[i].radius;
	
		float	discriminant = b * b - 4 * a * c;
	
		if (discriminant >= 0.0f)
		{
			float	t = (-b - sqrtf(discriminant)) / (2.0f * a);
			if (t > 0.0f && t < hitinfo.distance)
			{
				hitinfo.hit = true;
				hitinfo.distance = t;
				hitinfo.position = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
				hitinfo.normal = vec3_normalize(vec3_sub(hitinfo.position, spheres[i].position));
				hitinfo.material = spheres[i].material;
			}
		}
		i++;
	}
	return (hitinfo);
}

t_hitinfo	plane_intersection(t_ray ray, t_plane *planes, t_hitinfo obj_hit)
{
	// printf("plane_intersection\n");
	int	i;

	i = 0;
	for (int i = 0; i < array_length(&planes); i++)
	{
		float denom = vec3_dot(ray.direction, planes[i].normal);
		if (fabs(denom) > 1e-6)
		{
			t_vec3 p0l0 = vec3_sub(planes[i].position, ray.origin);
			float t = vec3_dot(p0l0, planes[i].normal) / denom;
			if (t >= 0.0f)
			{
				if (t < obj_hit.distance)
				{
					t_vec3 intersection_point = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
					float half_width = planes[i].width / 2.0f;
					float half_height = planes[i].height / 2.0f;
					
					// Check if the intersection point lies within the plane's limits
					if (intersection_point.x >= (planes[i].position.x - half_width) &&
						intersection_point.x <= (planes[i].position.x + half_width) &&
						intersection_point.y >= (planes[i].position.y - half_height) &&
						intersection_point.y <= (planes[i].position.y + half_height) &&
						intersection_point.z >= (planes[i].position.z - half_height) &&
						intersection_point.z <= (planes[i].position.z + half_height))
					{
						// if (vec3_dot(ray.direction, planes[i].normal) <= 0.0f)
						if (vec3_dot(ray.direction, planes[i].normal) < 0.0f)
						{
							obj_hit.hit = true;
							// obj_hit.distance = t;
							// obj_hit.position = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
							// // obj_hit.normal = vec3_normalize(vec3_sub(obj_hit.position, planes[i].position));
							// obj_hit.normal = planes[i].normal;
							obj_hit.material = planes[i].material;
						}
					}
				}
			}
		}
	}
	return (obj_hit);
}

// t_hitinfo	plane_intersection(t_ray ray, t_plane *planes, t_hitinfo obj_hit)
// {
// 	for (int i = 0; i < array_length(&planes); i++)
// 	{
// 		float denom = vec3_dot(ray.direction, planes[i].normal);
// 		if (fabs(denom) > 1e-6)
// 		{
// 			t_vec3 p0l0 = vec3_sub(planes[i].position, ray.origin);
// 			float t = vec3_dot(p0l0, planes[i].normal) / denom;
// 			if (t < 0.0f)
// 				continue;
// 			if (t < obj_hit.distance)
// 			{
// 				t_vec3 intersection_point = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
// 				float half_width = planes[i].width / 2.0f;
// 				float half_height = planes[i].height / 2.0f;
				
// 				// Check if the intersection point lies within the plane's limits
// 				if (intersection_point.x >= (planes[i].position.x - half_width) &&
// 					intersection_point.x <= (planes[i].position.x + half_width) &&
// 					intersection_point.y >= (planes[i].position.y - half_height) &&
// 					intersection_point.y <= (planes[i].position.y + half_height) &&
// 					intersection_point.z >= (planes[i].position.z - half_height) &&
// 					intersection_point.z <= (planes[i].position.z + half_height))
// 				{
// 					bool entersFromOpposite = vec3_dot(ray.direction, planes[i].normal) > 0.0f;
// 					if (entersFromOpposite)
// 						 continue;  // Ignore the intersection
// 					obj_hit.hit = true;
// 					obj_hit.distance = t;
// 					obj_hit.position = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
// 					// obj_hit.normal = vec3_normalize(vec3_sub(obj_hit.position, planes[i].position));
// 					obj_hit.normal = planes[i].normal;
// 					obj_hit.material = planes[i].material;
// 				}
// 			}
// 		}
// 	}
// 	return (obj_hit);
// }

int clamp(int value, int min, int max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

#define EPSILON 0.00001f

//! sabastion
t_hitinfo triangle_intersection(t_ray ray, t_hitinfo obj_hit, t_tri tri)
{
    t_vec3 e1 = vec3_sub(tri.b, tri.a); // tri.b - tri.a
	t_vec3 e2 = vec3_sub(tri.c, tri.a); // tri.a - tri.c
	// t_vec3 n = vec3_cross(e1, e2);
	t_vec3 h = vec3_cross(ray.direction, e2);
	float a = vec3_dot(e1, h);

	if (a > -EPSILON && a < EPSILON)
		return (obj_hit);

	float f = 1.0f / a;
	t_vec3 s = vec3_sub(ray.origin, tri.a);
	float u = f * vec3_dot(s, h);

	if (u < 0.0f || u > 1.0f)
		return (obj_hit);

	t_vec3 q = vec3_cross(s, e1);
	float v = f * vec3_dot(ray.direction, q);

	if (v < 0.0f || u + v > 1.0f)
		return (obj_hit);
	
	float t = f * vec3_dot(e2, q);

	float w = 1.0f - u - v;
	
	if (t > EPSILON && t < obj_hit.distance)
	{

		// int textureWidth = uv_tex->width;
		// int textureHeight = uv_tex->height;
		
		// // Barycentric Interpolation for texture coordinates
		// float texU = (1 - u - v) * tri.uv_a.x + u * tri.uv_b.x + v * tri.uv_c.x;
		// float texV = (1 - u - v) * tri.uv_a.y + u * tri.uv_b.y + v * tri.uv_c.y;
		
		// // Wrap texture coordinates to [0, 1] (or tile the texture)
		// // texU = fmod(texU, 1.0f);
		// // texV = fmod(texV, 1.0f);
		// texU = fmod(texU + 1.0f, 1.0f);
		// texV = fmod(texV + 1.0f, 1.0f);

		// if (texU < 0)
		//     texU += 1.0f;
		// if (texV < 0)
		//     texV += 1.0f;
		
		// int x = (int)(texU * textureWidth);
		// int y = (int)(texV * textureHeight);
		
		// int pixelIndex = (y * textureWidth + x) * 4; // 4 channels (R, G, B, A) per pixel
		
		// uint8_t r = uv_tex->pixels[pixelIndex];
		// uint8_t g = uv_tex->pixels[pixelIndex + 1];
		// uint8_t b = uv_tex->pixels[pixelIndex + 2];




		obj_hit.hit = true;
		obj_hit.distance = t;
		// obj_hit.position = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
		obj_hit.normal = vec3_normalize(vec3_cross(e1, e2));
		obj_hit.material.color = vec3_new(0.5, 0.5, 0.5);
		return obj_hit;

		float r = fabs(obj_hit.normal.x);
		float g = fabs(obj_hit.normal.y);
		float b = fabs(obj_hit.normal.z);
		if (obj_hit.normal.y < 0)
			r = 0.8;
		obj_hit.material.color = vec3_new(r, g, b);

		// obj_hit.material.color = vec3_new(r / 255.0f, g / 255.0f, b / 255.0f);
	}
	
    return obj_hit;
}

t_hitinfo intersectRayPlane(t_ray ray, float yLevel, t_hitinfo hitinfo)
{
	float	t;
	
	// If the ray is parallel to the plane (direction.y is 0), there is no intersection
	if (ray.direction.y == 0)
		hitinfo;
		

	t = (yLevel - ray.origin.y) / ray.direction.y;

	// If the intersection point is behind the ray, there is no intersection
	if (t < 0)
		hitinfo;


	hitinfo.hit = true;
	hitinfo.distance = t;
	hitinfo.position = vec3_add(ray.origin, vec3_mulf(ray.direction, t + 0.0001f));
	hitinfo.material.color = vec3_new(1.0, 0.5, 0.5);
	return hitinfo;
}
