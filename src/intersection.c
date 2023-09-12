/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/09/12 23:09:29 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"
// # include "lib3d.h"

t_hitinfo	sphere_intersection(t_ray ray, t_sphere sphere)
{
	t_hitinfo	hitinfo = {0}; // Initialize all members to zero (null)

	t_vec3	offset_origin = vec3_sub(ray.origin, sphere.position);

	float	a = vec3_dot(ray.direction, ray.direction);
	float	b = 2.0f * vec3_dot(offset_origin, ray.direction);
	float	c = vec3_dot(offset_origin, offset_origin) - sphere.radius * sphere.radius;

	float	discriminant = b * b - 4 * a * c;

	if (discriminant >= 0)
	{
		// printf("discriminant: %f\n", discriminant);
		float	t = (-b - sqrtf(discriminant)) / (2.0f * a);

		if (t >= 0)
		{
			hitinfo.distance = t;
			hitinfo.position = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
			hitinfo.normal = vec3_normalize(vec3_sub(hitinfo.position, sphere.position));
			hitinfo.hit = true;
			return (hitinfo);
		}
	}
	return (hitinfo);
}