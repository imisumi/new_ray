/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 02:06:12 by ichiro            #+#    #+#             */
/*   Updated: 2023/09/12 23:45:47 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"

t_sphere create_sphere(t_vec3 center, float radius)
{
	t_sphere s;

	s.position = center;
	s.radius = radius;

	s.material.color = vec4_new(1.0f, 0.0f, 1.0f, 1.0f);
	return (s);
}

void	init_scene_one(t_scene *scene)
{
	t_sphere	sphere;

	vec_init(&scene->spheres, 16, sizeof(t_sphere));
	// scene->spheres[0] = create_sphere(vec3_new(0.0f, 0.0f, 0.0f), 1.0f);
	sphere = create_sphere(vec3_new(0.0f, 0.0f, 0.0f), 0.5f);
	array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(0.0f, 2.0f, 0.0f), 0.5f);
	array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(-1.0f, 2.0f, 0.0f), 0.5f);
	array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(-1.0f, 0.0f, 0.0f), 0.5f);
	array_push(&scene->spheres, &sphere);
	// exit(0);
}