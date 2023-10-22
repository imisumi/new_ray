/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 02:06:12 by ichiro            #+#    #+#             */
/*   Updated: 2023/10/22 04:17:38 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/scene.h"

// #include "../lib/ft_math/includes/math.h"

#include "../includes/main.h"

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdint.h>
# include <unistd.h>
# include <string.h>
# include <math.h>

// extern uint32_t accumulated_frames;

// t_vec2	random_point_in_circle()
extern int	total_frames;

float randomFloat(uint32_t *state);

t_vec2	random_point_in_circle(uint32_t *state)
{
	float angle = randomFloat(state) * 2.0f * M_PI;
	t_vec2 point_on_circle = vec2_new(cosf(angle), sinf(angle));
	return (vec2_mulf(point_on_circle, sqrtf(randomFloat(state))));
}

int global_frame = 0;
void recalculat_ray_directions(t_data *d)
{
	for (uint32_t y = 0; y < HEIGHT; y++)
	{
		for (uint32_t x = 0; x < WIDTH; x++)
		{
			t_vec2 coord = {(float)x / (float)(WIDTH), (float)y / (float)(HEIGHT)};
			coord = vec2_subf(vec2_mulf(coord, 2.0), 1.0f);
			t_vec4 target = mat4_mul_vec4(d->scene.camera.inv_projection, vec4_new(coord.x, coord.y, 1, 1));
			t_vec3 t = vec3_divf(vec3_new(target.x, target.y, target.z), target.w);
			target = vec4_normalize(vec4_new(-t.x, t.y, -t.z, 0.0f));
			target = mat4_mul_vec4(d->scene.camera.inv_view, target);
			d->scene.camera.ray_target[x + y * WIDTH] = target;
			t_vec3 rayDirection = vec3_new(target.x, target.y, target.z);
			d->scene.camera.ray_dir[x + y * WIDTH] = rayDirection;

		}
	}
}

void anti_aliasing(t_data *d)
{
	uint32_t rngState;
	float diverge = 1.0f;
	for (uint32_t y = 0; y < HEIGHT; y++)
	{
		for (uint32_t x = 0; x < WIDTH; x++)
		{
			t_vec2 coord = {(float)x / (float)(WIDTH), (float)y / (float)(HEIGHT)};
			t_vec4 target = d->scene.camera.ray_target[x + y * WIDTH];

			t_vec2 numPixels = vec2_new((float)WIDTH, (float)HEIGHT);
			t_vec2 pixelCoord = vec2_mul(coord, numPixels);
			uint32_t pixelIndex = pixelCoord.x + pixelCoord.y * numPixels.x;
			rngState = pixelIndex + global_frame * 719393;

			t_vec3	cam_right = vec3_new(
				d->scene.camera.view.m[0][0],
				d->scene.camera.view.m[1][0],
				d->scene.camera.view.m[2][0]
			);
			t_vec3	cam_up = vec3_new(
				d->scene.camera.view.m[0][1],
				d->scene.camera.view.m[1][1],
				d->scene.camera.view.m[2][1]
			);
			t_vec2 jitter = vec2_divf(vec2_mulf(random_point_in_circle(&rngState), diverge), numPixels.x);
			// printf("jitter: %f, %f\n", jitter.x, jitter.y);
			t_vec3 jittered = vec3_add(vec3_new(target.x, target.y, target.z) ,vec3_add(vec3_mulf(cam_right, jitter.x), vec3_mulf(cam_up, jitter.y)));
			d->scene.camera.ray_dir[x + y * WIDTH] = vec3_normalize(jittered);
			global_frame++;
		
		}
	}
}

void init_camera(t_camera *cam)
{
	cam->position = vec3_new(0.0, 0.0, -3.0f);
	cam->direction = vec3_new(0.0, 0.0, -1.0);

	cam->ray_dir = malloc(sizeof(t_vec3) * WIDTH * HEIGHT);

	cam->mouse_delta.x = 0;
	cam->mouse_delta.y = 0;

	cam->projection = mat4_identity();
	cam->inv_projection = mat4_identity();
	cam->view = mat4_identity();
	cam->inv_view = mat4_identity();

	cam->vertical_fov = 45.0f;
	cam->aspectRatio = (float)WIDTH / (float)HEIGHT;
	cam->zNear = 0.1f;
	cam->zFar = 100.0f;

	cam->prev_mouse_pos.x = -1;
	cam->prev_mouse_pos.y = -1;
}

void recalculate_view(t_data *d)
{
	d->scene.camera.view = mat4_look_at(d->scene.camera.position, \
							vec3_add(d->scene.camera.position, d->scene.camera.direction), \
							vec3_new(0.0f, 1.0f, 0.0f));
	d->scene.camera.inv_view = mat4_inverse(d->scene.camera.view);
}

void recalculated_projection(t_data *d)
{
	d->scene.camera.projection = mat4_perspective(fov_radians(d->scene.camera.vertical_fov), \
								(float)WIDTH / (float)HEIGHT, \
								d->scene.camera.zNear, d->scene.camera.zFar);
	d->scene.camera.inv_projection = mat4_inverse(d->scene.camera.projection);
}

void	movement(t_data *d)
{
	bool moved = false;
	bool rotated = false;
	t_vec3 temp;
	t_vec3 up_direction = vec3_new(0.0f, 1.0f, 0.0f);
	t_vec3 right_direction = vec3_cross(d->scene.camera.direction, up_direction);

	float speed = 1.25f;

	if (!mlx_is_mouse_down(d->mlx, MLX_MOUSE_BUTTON_RIGHT))
	{
		d->scene.camera.mouse_lock = false;
		d->scene.camera.prev_mouse_pos.x = -1;
		d->scene.camera.prev_mouse_pos.y = -1;
		d->scene.camera.mouse_pos.x = 0;
		d->scene.camera.mouse_pos.y = 0;
		return ;
	}
	if (mlx_is_key_down(d->mlx, MLX_KEY_W)) {
		temp = vec3_mulf(d->scene.camera.direction, speed);
		d->scene.camera.position = vec3_add(d->scene.camera.position, temp);
		moved = true;
	}
	if (mlx_is_key_down(d->mlx, MLX_KEY_S)) {
		temp = vec3_mulf(d->scene.camera.direction, speed);
		d->scene.camera.position = vec3_sub(d->scene.camera.position, temp);
		moved = true;
	}
	if (mlx_is_key_down(d->mlx, MLX_KEY_A)) {
		temp = vec3_mulf(right_direction, speed);
		d->scene.camera.position = vec3_sub(d->scene.camera.position, temp);
		moved = true;
	}
	if (mlx_is_key_down(d->mlx, MLX_KEY_D)) {
		temp = vec3_mulf(right_direction, speed);
		d->scene.camera.position = vec3_add(d->scene.camera.position, temp);
		moved = true;
	}
	if (mlx_is_key_down(d->mlx, MLX_KEY_SPACE)) {
		temp = vec3_mulf(up_direction, speed);
		// d->scene.camera.position = vec3_sub(d->scene.camera.position, temp);
		d->scene.camera.position = vec3_add(d->scene.camera.position, temp);
		moved = true;
	}
	if (mlx_is_key_down(d->mlx, MLX_KEY_LEFT_SHIFT)) {
		temp = vec3_mulf(up_direction, speed);
		// d->scene.camera.position = vec3_add(d->scene.camera.position, temp);
		d->scene.camera.position = vec3_sub(d->scene.camera.position, temp);
		moved = true;
	}
	if (d->scene.camera.prev_mouse_pos.x >= 0 && d->scene.camera.prev_mouse_pos.y >= 0)
	{
		int x;
		int y;
		mlx_get_mouse_pos(d->mlx, &x, &y);
		d->scene.camera.mouse_pos = vec2_sub(d->scene.camera.prev_mouse_pos, vec2_new((int)x, (int)y));
		// printf("mouse_pos: %f, %f\n", d->scene.camera.mouse_pos.x, d->scene.camera.mouse_pos.y);
		
		if (d->scene.camera.mouse_pos.x != d->scene.camera.prev_mouse_pos.x && d->scene.camera.mouse_pos.y != d->scene.camera.prev_mouse_pos.y)
		{
			// rotated = true;

			d->scene.camera.mouse_delta.x += -1 * d->scene.camera.mouse_pos.x * 0.12f;
			d->scene.camera.mouse_delta.y +=  d->scene.camera.mouse_pos.y * 0.12f;
		}
		
	}
	int x;
	int y;
	mlx_get_mouse_pos(d->mlx, &x, &y);
	d->scene.camera.prev_mouse_pos = vec2_new((int)x, (int)y);

	// TODO: rotation

	if (d->scene.camera.mouse_delta.x != 0 || d->scene.camera.mouse_delta.y != 0)
	{
		// printf("HELLO\n");
		float pitch = d->scene.camera.mouse_delta.y * -0.01f;
		float yaw = d->scene.camera.mouse_delta.x * 0.01f;
		
		t_quat pitchRotation = quat_angle_axis(-pitch, right_direction);
		t_quat yawRotation = quat_angle_axis(-yaw, up_direction);
		t_quat q = quat_normalize(quat_cross(pitchRotation, yawRotation));
			
		// Rotate the forward direction using the quaternion
		t_vec3 rotatedForwardDirection = quat_rotate(q, d->scene.camera.direction);
			
		// Update the camera's forward direction
		d->scene.camera.direction = rotatedForwardDirection;
		d->scene.camera.mouse_delta.x = 0;
		d->scene.camera.mouse_delta.y = 0;
		moved = true;
		rotated = true;
	}
	
	if (moved) {
		// accumulated_frames = 1;
		d->utils.accumulated_frames = 1;
		recalculate_view(d);
		if (rotated) {
			recalculat_ray_directions(d);
		}
	}
}

// float4 frag (v2f i) : SV_Target
// {
// 	return i.uv.x;
// }