/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/10/24 00:57:53 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"

t_vert	*vert_cube()
{
	t_vert		*vert;
	t_vert		vec;

	vec_init(&vert, 16, sizeof(t_vert));
	vec.position = vec3_new(-1.000000, -1.000000, 1.000000);
	array_push(&vert, &vec.position);
	vec.position = vec3_new(-1.000000, 1.000000, 1.000000);
	array_push(&vert, &vec.position);
	vec.position = vec3_new(1.000000, -1.000000, 1.000000);
	array_push(&vert, &vec.position);
	vec.position = vec3_new(1.000000, 1.000000, 1.000000);
	array_push(&vert, &vec.position);
	vec.position = vec3_new(1.000000, -1.000000, -1.000000);
	array_push(&vert, &vec.position);
	vec.position = vec3_new(1.000000, 1.000000, -1.000000);
	array_push(&vert, &vec.position);
	vec.position = vec3_new(-1.000000, -1.000000, -1.000000);
	array_push(&vert, &vec.position);
	vec.position = vec3_new(-1.000000, 1.000000, -1.000000);
	array_push(&vert, &vec.position);
	return (vert);
}

t_tri_faces	*cube_faces()
{
	t_tri_faces	*faces;
	t_tri_faces	tri;

	vec_init(&faces, 16, sizeof(t_tri_faces));
	tri.index[0] = vec3_new(1, 2, 0);
	tri.index[1] = vec3_new(7, 1, 0);
	tri.index[2] = vec3_new(0, 0, 0);
	array_push(&faces, &tri);
	tri.index[0] = vec3_new(5, 2, 1);
	tri.index[1] = vec3_new(3, 1, 1);
	tri.index[2] = vec3_new(4, 0, 1);
	array_push(&faces, &tri);
	tri.index[0] = vec3_new(5, 1, 2);
	tri.index[1] = vec3_new(4, 3, 2);
	tri.index[2] = vec3_new(7, 2, 2);
	array_push(&faces, &tri);
	tri.index[0] = vec3_new(0, 0, 0);
	tri.index[1] = vec3_new(7, 1, 0);
	tri.index[2] = vec3_new(6, 3, 0);
	array_push(&faces, &tri);
	tri.index[0] = vec3_new(5, 2, 3);
	tri.index[1] = vec3_new(7, 1, 3);
	tri.index[2] = vec3_new(3, 0, 3);
	array_push(&faces, &tri);
	tri.index[0] = vec3_new(0, 1, 4);
	tri.index[1] = vec3_new(6, 3, 4);
	tri.index[2] = vec3_new(2, 2, 4);
	array_push(&faces, &tri);
	tri.index[0] = vec3_new(2, 0, 5);
	tri.index[1] = vec3_new(3, 2, 5);
	tri.index[2] = vec3_new(0, 3, 5);
	array_push(&faces, &tri);
	tri.index[0] = vec3_new(1, 1, 5);
	tri.index[1] = vec3_new(0, 3, 5);
	tri.index[2] = vec3_new(3, 2, 5);
	array_push(&faces, &tri);
	tri.index[0] = vec3_new(4, 0, 1);
	tri.index[1] = vec3_new(3, 1, 1);
	tri.index[2] = vec3_new(2, 3, 1);
	array_push(&faces, &tri);
	tri.index[0] = vec3_new(6, 0, 2);
	tri.index[1] = vec3_new(7, 2, 2);
	tri.index[2] = vec3_new(4, 3, 2);
	array_push(&faces, &tri);
	tri.index[0] = vec3_new(3, 0, 3);
	tri.index[1] = vec3_new(7, 1, 3);
	tri.index[2] = vec3_new(1, 3, 3);
	array_push(&faces, &tri);
	tri.index[0] = vec3_new(4, 0, 4);
	tri.index[1] = vec3_new(2, 2, 4);
	tri.index[2] = vec3_new(6, 3, 4);
	array_push(&faces, &tri);
	
	
	return (faces);
}