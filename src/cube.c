/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/09/29 16:10:30 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"

t_cube	scale_cube(t_cube cube, float sx, float sy, float sz)
{
	for (int i = 0; i < 6; i++)
		cube.faces[i] = scale_quad(cube.faces[i], sx, sy, sz);
	return (cube);
}

t_cube	translate_cube(t_cube cube, float sx, float sy, float sz)
{
	for (int i = 0; i < 6; i++)
		cube.faces[i] = translate_quad(cube.faces[i], sx, sy, sz);
	return (cube);
}

t_cube	rotate_cube(t_cube cube, float rx, float ry, float rz)
{
	for (int i = 0; i < 6; i++)
		cube.faces[i] = rotate_quad(cube.faces[i], rx, ry, rz);
	return (cube);
}

t_cube	init_cube(void)
{
	t_cube	c;
	t_quad	q;

	q = init_quad();
	q = rotate_quad(q, 90.0f, 0.0f, 0.0f);
	q = translate_quad(q, 0.0f, 0.0f, 1.0f);
	c.faces[0] = q;
	q = init_quad();
	q = rotate_quad(q, 90.0f, 0.0f, -90.0f);
	q = translate_quad(q, 1.0f, 0.0f, 0.0f);
	c.faces[1] = q;
	q = init_quad();
	q = rotate_quad(q, 90.0f, 0.0f, 180.0f);
	q = translate_quad(q, 0.0f, 0.0f, -1.0f);
	c.faces[2] = q;
	q = init_quad();
	q = rotate_quad(q, 90.0f, 0.0f, 90.0f);
	q = translate_quad(q, -1.0f, 0.0f, 0.0f);
	c.faces[3] = q;
	q = init_quad();
	q = translate_quad(q, 0.0f, 1.0f, 0.0f);
	c.faces[4] = q;
	q = init_quad();
	q = rotate_quad(q, 180.0f, 0.0f, 0.0f);
	q = translate_quad(q, 0.0f, -1.0f, 0.0f);
	c.faces[5] = q;
	return (c);
}