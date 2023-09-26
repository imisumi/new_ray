/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 02:06:12 by ichiro            #+#    #+#             */
/*   Updated: 2023/09/26 17:32:48 by imisumi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"

t_sphere create_sphere(t_vec3 center, float radius)
{
	t_sphere s;

	s.position = center;
	s.radius = radius;

	s.material.color = vec3_new(1.0f, 1.0f, 1.0f);
	s.material.emission_color = vec3_new(0.0f, 0.0f, 0.0f);
	s.material.emission_strength = 0.0f;
	return (s);
}

t_plane	create_plane(float w, float h, t_vec3 p, t_vec3 n, t_material m)
{
	t_plane	plane;

	plane.height = h;
	plane.width = w;
	plane.position = p;
	plane.normal = n;
	plane.material = m;
	return (plane);
}

t_material	new_diffuse(t_vec3 color)
{
	t_material	material;

	material.color = color;
	material.emission_color = vec3_new(0.0f, 0.0f, 0.0f);
	material.emission_strength = 0.0f;
	material.roughness = 0.0f;
	material.specular = 0.0f;
	material.specular_color = vec3_new(0.0f, 0.0f, 0.0f);
	return (material);
}

t_material	new_emission(t_vec3 col, float strenght)
{
	t_material	material;

	material.color = vec3_new(0.0f, 0.0f, 0.0f);
	material.emission_color = col;
	material.emission_strength = strenght;
	material.specular = 0.0f;
	material.specular_color = vec3_new(0.0f, 0.0f, 0.0f);
	return (material);
}

void	init_scene_one(t_scene *scene)
{
	t_sphere	sphere;

	vec_init(&scene->spheres, 16, sizeof(t_sphere));
	// scene->spheres[0] = create_sphere(vec3_new(0.0f, 0.0f, 0.0f), 1.0f);
	sphere = create_sphere(vec3_new(0.0f, -50.0f, 0.0f), 50.0f);
	array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(0.0f, 0.0f, 0.0f), 0.5f);
	array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(0.0f, 2.0f, 0.0f), 0.5f);
	array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(-1.0f, 2.0f, 0.0f), 0.5f);
	array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(-1.0f, 0.0f, 0.0f), 0.5f);
	array_push(&scene->spheres, &sphere);
	
	


	scene->spheres[0].material = new_diffuse(vec3_new(0.365f, 0.87f, 0.749f));
	
	// scene->spheres[0].material.color = vec3_new(0.0f, 0.0f, 0.0f);
	// scene->spheres[0].material.emission_color = vec3_new(1.0f, 1.0f, 1.0f);
	// scene->spheres[0].material.emission_strength = 102.1f;
	// exit(0);
}

void	init_scene_two(t_scene *scene)
{
	t_sphere	sphere;
	t_plane		plane;

	vec_init(&scene->spheres, 16, sizeof(t_sphere));
	vec_init(&scene->planes, 32, sizeof(t_plane));
	// scene->spheres[0] = create_sphere(vec3_new(0.0f, 0.0f, 0.0f), 1.0f);

	//! bottom
	plane = create_plane(5.1f, 3.1f, \
							vec3_new(0.0f, 0.0f, 0.0f), \
							vec3_up(), \
							new_diffuse(vec3_new(0.4f, 0.4f, 0.4f)));
	array_push(&scene->planes, &plane);
	//! top
	plane = create_plane(5.1f, 3.1f, \
							vec3_new(0.0f, 3.0f, 0.0f), \
							vec3_new(0.0f,-1.0f, 0.0f), \
							new_diffuse(vec3_new(0.9f, 0.9f, 0.9f)));
	array_push(&scene->planes, &plane);
	//! left
	plane = create_plane(3.1f, 3.1f, \
							vec3_new(-2.5f, 1.5f, 0.0f), \
							vec3_new(1.0f, 0.0f, 0.0f), \
							new_diffuse(vec3_new(0.478f, 0.792f, 0.949f)));
	array_push(&scene->planes, &plane);
	// ! right
	plane = create_plane(5.1f, 3.1f, \
							vec3_new(2.5f, 1.5f, 0.0f), \
							vec3_new(-1.0f, 0.0f, 0.0f), \
							new_diffuse(vec3_new(0.941f, 0.4787f, 0.949f)));
	array_push(&scene->planes, &plane);
	//! front
	plane = create_plane(5.1f, 3.1f, \
							vec3_new(0.0f, 1.5f, 1.5f), \
							vec3_new(0.0f, 0.0f, -1.0f), \
							new_diffuse(vec3_new(1.0f, 1.0f, 1.0f)));
	array_push(&scene->planes, &plane);
	//! back
	plane = create_plane(5.1f, 3.1f, \
							vec3_new(0.0f, 1.5f, -1.5f), \
							vec3_new(0.0f, 0.0f, 1.0f), \
							new_diffuse(vec3_new(0.365f, 0.87f, 0.749f)));
	array_push(&scene->planes, &plane);



	//! light
	plane = create_plane(2.1f, 2.1f, \
							vec3_new(0.0f, 2.999f, 0.0f), \
							vec3_new(0.0f,-1.0f, 0.0f), \
							new_diffuse(vec3_new(0.0, 0.0, 0.0)));
	array_push(&scene->planes, &plane);
	scene->planes[6].material = new_emission(vec3_new(1.0f, 1.0f, 1.0f), 3.1f);
	
	// scene->spheres[0].material = new_diffuse(vec3_new(0.365f, 0.87f, 0.749f));

	sphere = create_sphere(vec3_new(-1.5f, 1.5f, 0.0f), 0.4f);
	array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(-0.5f, 1.5f, 0.0f), 0.4f);
	array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(0.5f, 1.5f, 0.0f), 0.4f);
	array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(1.5f, 1.5f, 0.0f), 0.4f);
	array_push(&scene->spheres, &sphere);

	scene->spheres[0].material.roughness = 1.0f;
	scene->spheres[0].material.specular = 1.0f;
	scene->spheres[0].material.specular_color = vec3_new(1.0f, 1.0f, 1.0f);

	scene->spheres[1].material.roughness = 1.0f;
	scene->spheres[1].material.specular = 0.4f;
	scene->spheres[1].material.specular_color = vec3_new(1.0f, 1.0f, 1.0f);
	scene->spheres[1].material.color = vec3_new(1.0f, 0.0f, 0.0f);

	scene->spheres[2].material.roughness = 1.0f;
	scene->spheres[2].material.specular = 0.15f;
	scene->spheres[2].material.specular_color = vec3_new(1.0f, 1.0f, 1.0f);
	
	scene->spheres[3].material.roughness = 1.0f;
	// scene->spheres[3].material.color = vec3_new(1.0f, 0.0f, 0.0f);
	scene->spheres[3].material.specular = 0.02f;
	scene->spheres[3].material.specular_color = vec3_new(1.0f, 1.0f, 1.0f);
	
	
	// scene->spheres[0].material.color = vec3_new(0.0f, 0.0f, 0.0f);
	// scene->spheres[0].material.emission_color = vec3_new(1.0f, 1.0f, 1.0f);
	// scene->spheres[0].material.emission_strength = 102.1f;
	// exit(0);
}

void	init_scene_three(t_scene *scene)
{
	t_sphere	sphere;

	vec_init(&scene->spheres, 16, sizeof(t_sphere));
	vec_init(&scene->planes, 32, sizeof(t_plane));

	sphere = create_sphere(vec3_new(0.0f, 1.3f, 0.0f), 1.0f);
	sphere.material = new_diffuse(vec3_new(1.0f, 0.0f, 0.0f));
	array_push(&scene->spheres, &sphere);
	

}

void	init_scene_five(t_scene *scene)
{
	t_sphere	sphere;
	t_plane		plane;

	vec_init(&scene->spheres, 16, sizeof(t_sphere));
	vec_init(&scene->planes, 32, sizeof(t_plane));
	// return ;
	// sphere = create_sphere(vec3_new(0.0f, -50.0f, 0.0f), 50.0f);
	// array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(-9.0f, -9.5f, -20.0f), 3.0f);
	array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(0.0f, -9.5f, -20.0f), 3.0f);
	array_push(&scene->spheres, &sphere);
	sphere = create_sphere(vec3_new(9.0f, -9.5f, -20.0f), 3.0f);
	array_push(&scene->spheres, &sphere);
	// sphere = create_sphere(vec3_new(10.0f, 10.0f, -20.0f), 5.0f);
	// array_push(&scene->spheres, &sphere);

	//! back
	plane = create_plane(25.1f, 25.1f, \
							vec3_new(0.0f, 0.0f, -25.0f), \
							vec3_new(0.0f, 0.0f, 1.0f), \
							new_diffuse(vec3_new(0.7f, 0.7f, 0.7f)));
	array_push(&scene->planes, &plane);
	//! floor
	plane = create_plane(25.1f, 10.1f, \
							vec3_new(0.0f, -12.45f, -20.0f), \
							vec3_new(0.0f, 1.0f, 0.0f), \
							new_diffuse(vec3_new(0.7f, 0.7f, 0.7f)));
	array_push(&scene->planes, &plane);
	//! roof
	plane = create_plane(25.1f, 10.1f, \
							vec3_new(0.0f, 12.45f, -20.0f), \
							vec3_new(0.0f, -1.0f, 0.0f), \
							new_diffuse(vec3_new(0.7f, 0.7f, 0.7f)));
	array_push(&scene->planes, &plane);
	// ! right
	plane = create_plane(0.1f, 25.1f, \
							vec3_new(12.5f, 0.0f, -27.5f), \
							vec3_new(-1.0f, 0.0f, 0.0f), \
							new_diffuse(vec3_new(0.1f, 0.7f, 0.1f)));
	array_push(&scene->planes, &plane);
	// ! left
	plane = create_plane(5.1f, 25.1f, \
							vec3_new(-12.5f, 0.0f, -27.5f), \
							vec3_new(1.0f, 0.0f, 0.0f), \
							new_diffuse(vec3_new(0.7f, 0.1f, 0.1f)));
	array_push(&scene->planes, &plane);

	//! roof light
	plane = create_plane(10.1f, 5.1f, \
							vec3_new(0.0f, 12.4f, -20.0f), \
							vec3_new(0.0f, -1.0f, 0.0f), \
							new_emission(vec3_new(1.0f, 0.9f, 0.7f), 10.0f));
	array_push(&scene->planes, &plane);
	
	


	scene->spheres[0].material = new_diffuse(vec3_new(0.9f, 0.9f, 0.75f));
	scene->spheres[1].material = new_diffuse(vec3_new(0.9f, 0.75f, 0.9f));
	scene->spheres[2].material = new_diffuse(vec3_new(0.75f, 0.9f, 0.9f));




	

	float x = -10.0f;
	float y = 0.0f;
	float z = -23.0f;
	for (int i = 0; i < 5; i++)
	{
		sphere = create_sphere(vec3_new(x, y, z), 1.75f);
		array_push(&scene->spheres, &sphere);
		x += 5.0f;
	}
	scene->spheres[3].material = new_diffuse(vec3_new(1.0f, 1.0f, 1.0f));
	scene->spheres[3].material.roughness = 1.0f;
	scene->spheres[3].material.specular = 1.0f;
	scene->spheres[3].material.specular_color = vec3_new(0.3f, 1.0f, 0.3f);

	scene->spheres[4].material = new_diffuse(vec3_new(1.0f, 1.0f, 1.0f));
	scene->spheres[4].material.roughness = 0.75f;
	scene->spheres[4].material.specular = 1.0f;
	scene->spheres[4].material.specular_color = vec3_new(0.3f, 1.0f, 0.3f);

	scene->spheres[5].material = new_diffuse(vec3_new(1.0f, 1.0f, 1.0f));
	scene->spheres[5].material.roughness = 0.5f;
	scene->spheres[5].material.specular = 1.0f;
	scene->spheres[5].material.specular_color = vec3_new(0.3f, 1.0f, 0.3f);

	scene->spheres[6].material = new_diffuse(vec3_new(1.0f, 1.0f, 1.0f));
	scene->spheres[6].material.roughness = 0.25f;
	scene->spheres[6].material.specular = 1.0f;
	scene->spheres[6].material.specular_color = vec3_new(0.3f, 1.0f, 0.3f);

	scene->spheres[7].material = new_diffuse(vec3_new(1.0f, 1.0f, 1.0f));
	scene->spheres[7].material.roughness = 0.0f;
	scene->spheres[7].material.specular = 1.0f;
	scene->spheres[7].material.specular_color = vec3_new(0.3f, 1.0f, 0.3f);

	
	// scene->spheres[3].material = new_emission(vec3_new(1.0f, 0.9f, 0.7), 20.0f);
	
	// scene->spheres[0].material.color = vec3_new(0.0f, 0.0f, 0.0f);
	// scene->spheres[0].material.emission_color = vec3_new(1.0f, 1.0f, 1.0f);
	// scene->spheres[0].material.emission_strength = 102.1f;
	// exit(0);
}

void	init_scene_six(t_scene *scene)
{
	t_sphere	sphere;
	t_plane		plane;

	vec_init(&scene->spheres, 16, sizeof(t_sphere));
	vec_init(&scene->planes, 32, sizeof(t_plane));
	sphere = create_sphere(vec3_new(0.0f, 0.0f, 0.0f), 5.0f);
	// array_push(&scene->spheres, &sphere);
	plane = create_plane(10.1f, 15.1f, \
							vec3_new(0.0f, 0.0f, 0.0f), \
							vec3_new(0.0f, 1.0f, 0.0f), \
							new_diffuse(vec3_new(1.0f, 0.7f, 0.7f)));
	array_push(&scene->planes, &plane);
}