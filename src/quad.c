/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quad.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/09/28 23:04:27 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"

//	A = 0.0
//	B = 0.1
//	C = 1.1
//	D = 1.0
//	A--D
//	|\ |
//	| \|
//	B--C
t_quad	init_quad(void)
{
	t_quad	q;
	float	y;
	
	y = 0.0f;
	q.tri1.a = vec3_new(-1.0f, y, -1.0f);
	q.tri1.b = vec3_new(-1.0f, y, 1.0f);
	q.tri1.c = vec3_new(1.0f, y, 1.0f);
	q.tri1.uv_a = vec2_new(0.0f, 0.0f);
	q.tri1.uv_b = vec2_new(0.0f, 1.0f);
	q.tri1.uv_c = vec2_new(1.0f, 1.0f);
	// q.tri1.scale = create_scaling_matrix(1.0f, 1.0f, 1.0f);
	// q.tri1.translate = create_translation_matrix(0.0f, 0.0f, 0.0f);

	q.tri2.a = q.tri1.a;
	q.tri2.b = q.tri1.c;
	q.tri2.c = vec3_new(1.0f, y, -1.0f);
	q.tri2.uv_a = q.tri1.uv_a;
	q.tri2.uv_b = q.tri1.uv_c;
	q.tri2.uv_c = vec2_new(1.0f, 0.0f);

	return (q);
}

t_quad	rotate_quad(t_quad quad, float rx, float ry, float rz)
{
	t_quat	rotate_x = create_rotation_x(rx);
	t_quat	rotate_y = create_rotation_y(ry);
	t_quat	rotate_z = create_rotation_z(rz);
	t_quat	rotation = quaternion_multiply(rotate_x, rotate_y);
	rotation = quaternion_multiply(rotation, rotate_z);
	quad.tri1.a = quat_rotate(rotation, quad.tri1.a);
	quad.tri1.b = quat_rotate(rotation, quad.tri1.b);
	quad.tri1.c = quat_rotate(rotation, quad.tri1.c);
	quad.tri2.a = quat_rotate(rotation, quad.tri2.a);
	quad.tri2.b = quat_rotate(rotation, quad.tri2.b);
	quad.tri2.c = quat_rotate(rotation, quad.tri2.c);
	return (quad);
}

t_quad	scale_quad(t_quad quad, float sx, float sy, float sz)
{
	t_mat4	m;

	m = create_scaling_matrix(sx, sy, sz);
	quad.tri1.a = mat4_mul_vec3(m, quad.tri1.a);
	quad.tri1.b = mat4_mul_vec3(m, quad.tri1.b);
	quad.tri1.c = mat4_mul_vec3(m, quad.tri1.c);
	quad.tri2.a = mat4_mul_vec3(m, quad.tri2.a);
	quad.tri2.b = mat4_mul_vec3(m, quad.tri2.b);
	quad.tri2.c = mat4_mul_vec3(m, quad.tri2.c);
	return (quad);
}

t_quad	translate_quad(t_quad quad, float sx, float sy, float sz)
{
	t_mat4	m;

	m = create_translation_matrix(sx, sy, sz);
	quad.tri1.a = mat4_mul_vec3(m, quad.tri1.a);
	quad.tri1.b = mat4_mul_vec3(m, quad.tri1.b);
	quad.tri1.c = mat4_mul_vec3(m, quad.tri1.c);
	quad.tri2.a = mat4_mul_vec3(m, quad.tri2.a);
	quad.tri2.b = mat4_mul_vec3(m, quad.tri2.b);
	quad.tri2.c = mat4_mul_vec3(m, quad.tri2.c);
	return (quad);
}