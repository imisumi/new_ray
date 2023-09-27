/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/09/27 00:03:48 by imisumi-wsl      ###   ########.fr       */
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

t_vec3 sample_sphere_texture(t_vec3 intersection_point, t_vec3 sphere_center, float sphere_radius);

t_hitinfo	sphere_intersection(t_ray ray, t_sphere *spheres, t_hitinfo hitinfo)
{
	int	i;

	i = 0;
	// t_hitinfo	hitinfo = {0};
	while (i < array_length(&spheres))
	{
		// t_hitinfo	hitinfo = {0}; // Initialize all members to zero (null)
	
		t_vec3	offset_origin = vec3_sub(ray.origin, spheres[i].position);
	
		float	a = vec3_dot(ray.direction, ray.direction);
		float	b = 2.0f * vec3_dot(offset_origin, ray.direction);
		float	c = vec3_dot(offset_origin, offset_origin) - spheres[i].radius * spheres[i].radius;
	
		float	discriminant = b * b - 4 * a * c;
	
		// if (discriminant < 0.0f)
		// {
		// 	i++;
		// 	continue;
		// }
		if (discriminant > 0.0f)
		{
			float	t = (-b - sqrtf(discriminant)) / (2.0f * a);
			if (t > 0.0f && t < hitinfo.distance)
			{
				hitinfo.hit = true;
				hitinfo.distance = t;
				hitinfo.position = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
				hitinfo.normal = vec3_normalize(vec3_sub(hitinfo.position, spheres[i].position));
				hitinfo.material = spheres[i].material;
				t_vec3 uv_col = sample_sphere_texture(hitinfo.position, spheres[i].position, spheres[i].radius);
				hitinfo.material.color = uv_col;
			}
		}
		i++;
	}
	return (hitinfo);
}

t_hitinfo	plane_intersection(t_ray ray, t_plane *planes, t_hitinfo obj_hit)
{
	// printf("plane_intersection\n");
	for (int i = 0; i < array_length(&planes); i++)
	{
		float denom = vec3_dot(ray.direction, planes[i].normal);
		if (fabs(denom) > 1e-6)
		{
			t_vec3 p0l0 = vec3_sub(planes[i].position, ray.origin);
			float t = vec3_dot(p0l0, planes[i].normal) / denom;
			if (t < 0.0f)
				continue;
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
					bool entersFromOpposite = vec3_dot(ray.direction, planes[i].normal) > 0.0f;
					if (entersFromOpposite)
						 continue;  // Ignore the intersection
					obj_hit.hit = true;
					obj_hit.distance = t;
					obj_hit.position = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
					// obj_hit.normal = vec3_normalize(vec3_sub(obj_hit.position, planes[i].position));
					obj_hit.normal = planes[i].normal;
					obj_hit.material = planes[i].material;
				}
			}
		}
	}
	return (obj_hit);
}

t_vec3 vec3_negate(t_vec3 v) {
    t_vec3 result;
    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;
    return result;
}

// t_hitinfo	triangle_intersection(t_ray ray, t_hitinfo obj_hit, t_vec3 a, t_vec3 c, t_vec3 b)
// {
// 	t_vec3 edgeAB = vec3_sub(b, a);
// 	t_vec3 edgeAC = vec3_sub(c, a);
// 	t_vec3 normalVect = vec3_cross(edgeAB, edgeAC);
// 	// t_vec3 normalVect = vec3_cross(edgeAC, edgeAB);
// 	// normalVect = vec3_negate(normalVect);
// 	t_vec3 ao = vec3_sub(ray.origin, a);
// 	t_vec3 dao = vec3_cross(ao, ray.direction);

// 	float determinant = -vec3_dot(ray.direction, normalVect);
// 	float invDeterminant = 1.0f / determinant;

// 	float dest = vec3_dot(ao, normalVect) * invDeterminant;
// 	float u = vec3_dot(dao, edgeAC) * invDeterminant;
// 	float v = -vec3_dot(dao, edgeAB) * invDeterminant;
// 	float w = 1.0f - u - v;

// 	if (determinant >= 1e-6 && dest >= 0.0f && u >= 0.0f && v >= 0.0f && w >= 0.0f)
// 	{
// 		// printf("dest: %f\n", dest);
// 		if (dest < obj_hit.distance)
// 		{
// 			obj_hit.hit = true;
// 			obj_hit.distance = dest;
// 			obj_hit.position = vec3_add(ray.origin, vec3_mulf(ray.direction, dest));
// 			obj_hit.normal = normalVect;
// 			obj_hit.material.color = vec3_new(1.0f, 0.0f, 0.0f);
// 		}
// 	}
// 	return (obj_hit);
// }


//! working with bug
// t_hitinfo	triangle_intersection(t_ray ray, t_hitinfo obj_hit, t_vec3 a, t_vec3 b, t_vec3 c)
// {
// 	t_vec3 edgeAB = vec3_sub(b, a);
// 	t_vec3 edgeAC = vec3_sub(c, a);
// 	t_vec3 normalVect = vec3_cross(edgeAB, edgeAC);

// 	float n_dot_d = vec3_dot(normalVect, ray.direction);
// 	if (fabs(n_dot_d) < 1e-6)
// 		return (obj_hit);
	
// 	float n_dot_ps = vec3_dot(normalVect, vec3_sub(a, ray.origin));
// 	float t = n_dot_ps / n_dot_d;

// 	t_vec3 planePoint = vec3_add(ray.origin, vec3_mulf(ray.direction, t));

// 	t_vec3 a_to_b = vec3_sub(b, a);
// 	t_vec3 b_to_c = vec3_sub(c, b);
// 	t_vec3 c_to_a = vec3_sub(a, c);

// 	t_vec3 a_to_point = vec3_sub(planePoint, a);
// 	t_vec3 b_to_point = vec3_sub(planePoint, b);
// 	t_vec3 c_to_point = vec3_sub(planePoint, c);

// 	t_vec3 a_test_vec = vec3_cross(a_to_b, a_to_point);
// 	t_vec3 b_test_vec = vec3_cross(b_to_c, b_to_point);
// 	t_vec3 c_test_vec = vec3_cross(c_to_a, c_to_point);

// 	bool a_normal = vec3_dot(a_test_vec, normalVect) > 0.0f;
// 	bool b_normal = vec3_dot(b_test_vec, normalVect) > 0.0f;
// 	bool c_normal = vec3_dot(c_test_vec, normalVect) > 0.0f;

// 	if (a_normal && b_normal && c_normal)
// 	{
// 		if (t < obj_hit.distance)
// 		{
// 			obj_hit.hit = true;
// 			obj_hit.distance = t;
// 			obj_hit.position = planePoint;
// 			obj_hit.normal = normalVect;
			
// 			// obj_hit.material.color = vec3_new(0.0f, 1.0f, 0.0f);
// 			obj_hit.material.color = normalVect;
// 			obj_hit.material.emission_strength = 0.0f;
// 			obj_hit.material.specular = 0.0f;
// 			obj_hit.material.roughness = 0.0f;
			
// 		}
// 	}

// 	return (obj_hit);
// }

//! good but still artifactc
#define DOUBLE_SIDED 1
// t_hitinfo	triangle_intersection(t_ray ray, t_hitinfo obj_hit, t_vec3 vert0, t_vec3 vert01, t_vec3 vert02)
// {
// 	t_vec3 eddge1 = vec3_sub(vert01, vert0);
// 	t_vec3 eddge2 = vec3_sub(vert02, vert0);

// 	t_vec3 cross_rayDir_edge2 = vec3_cross(ray.direction, eddge2);

// 	float determinant = vec3_dot(eddge1, cross_rayDir_edge2);

// 	if (DOUBLE_SIDED)
// 	{
// 		if (fabs(determinant) < EPSILON)
// 			return (obj_hit);
// 	}
// 	else
// 	{
// 		if (determinant < EPSILON)
// 			return (obj_hit);
// 	}

// 	float inv_dev = 1.0f / determinant;

// 	t_vec3 origin_min_vert0 = vec3_sub(ray.origin, vert0);

// 	float baryU = vec3_dot(origin_min_vert0, cross_rayDir_edge2) * inv_dev;
// 	if (baryU < 0.0f || baryU > 1.0f)
// 		return (obj_hit);

// 	t_vec3 cross_origMinVert0_edgel = vec3_cross(origin_min_vert0, eddge1);

// 	float baryV = vec3_dot(ray.direction, cross_origMinVert0_edgel) * inv_dev;
// 	if (baryV < 0.0f || baryU + baryV > 1.0f)
// 		return (obj_hit);

// 	float rayT = vec3_dot(eddge2, cross_origMinVert0_edgel) * inv_dev;
// 	// obj_hit.material.color = vec3_new(0.0f, 1.0f, 0.0f);
// 	// obj_hit.hit = true;
// 	// if (rayT > EPSILON && rayT < FAR_CLIP && fabs(obj_hit.distance - rayT) < EPSILON)
// 	if (rayT > EPSILON && rayT < FAR_CLIP)
// 	{
// 		obj_hit.hit = true;
// 		obj_hit.distance = rayT;
// 		// obj_hit.position = planePoint;
// 		// obj_hit.normal = normalVect;
		
// 		obj_hit.material.color = vec3_new(1.0f, 0.0f, 0.0f);
// 		// obj_hit.material.color = normalVect;
// 		obj_hit.material.emission_strength = 0.0f;
// 		obj_hit.material.specular = 0.0f;
// 		obj_hit.material.roughness = 0.0f;
		
// 	}

// 	return (obj_hit);
// }

t_hitinfo	triangle_intersection(t_ray ray, t_hitinfo obj_hit, t_vec3 vert0, t_vec3 vert01, t_vec3 vert02)
{
	t_vec3 E1 = vec3_sub(vert01, vert0);
	t_vec3 E2 = vec3_sub(vert02, vert0);
	t_vec3 T = vec3_sub(ray.origin, vert0);
	t_vec3 D = ray.direction;
	t_vec3 P = vec3_cross(D, E2);
	t_vec3 Q = vec3_cross(T, E1);

	float f = 1.0f / vec3_dot(P, E1);
	if (f < EPSILON)
		return (obj_hit);
	float t = f * vec3_dot(Q, E2);
	float u = f * vec3_dot(P, T);
	float v = f * vec3_dot(Q, D);

	if (u > -EPSILON && v > -EPSILON && u + v < 1.0f + EPSILON && t > EPSILON && t < obj_hit.distance)
	{
		if (t < obj_hit.distance)
		{
			obj_hit.hit = true;
			obj_hit.distance = t;
			obj_hit.position = vec3_add(ray.origin, vec3_mulf(ray.direction, t));
			obj_hit.normal = vec3_normalize(vec3_cross(E1, E2));
			obj_hit.material.color = vec3_new(1.0f, 0.0f, 0.0f);
			obj_hit.material.emission_strength = 0.0f;
			obj_hit.material.specular = 0.0f;
			obj_hit.material.roughness = 0.0f;
		}
	}
	return (obj_hit);
}