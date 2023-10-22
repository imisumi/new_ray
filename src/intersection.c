/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/10/13 18:23:32 by imisumi-wsl      ###   ########.fr       */
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