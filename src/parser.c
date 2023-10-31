/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 02:06:12 by ichiro            #+#    #+#             */
/*   Updated: 2023/10/30 21:40:13 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"

const char *table[] = {
	"A ",
	"A\t",
	"C ",
	"C\t",
	"L ",
	"L\t",
	"pl ",
	"pl\t",
	"sp ",
	"sp\t",
	"cy ",
	"cy\t"
};

const char *allowed_chars[] = {
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"-",
	",",
	".",
	" ",
	"\t",
	"\n",
	"\0"
};

const int table_size = sizeof(table) / sizeof(char *);
const int chars_size = sizeof(allowed_chars) / sizeof(char *);

bool	check_allowed_characters(char *line)
{
	int	i;
	int	j;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\t')
			line[i] = ' ';
		j = 0;
		while (allowed_chars[j])
		{
			if (line[i] == allowed_chars[j][0])
				break ;
			j++;
		}
		if (j == chars_size - 1)
			return (false);
		i++;
	}
	return (true);
}

bool	valid_identifier(char *line)
{
	int	i;

	i = 0;
	while (i < table_size)
	{
		if (strncmp(line, table[i], strlen(table[i])) == 0)
		{
			return (check_allowed_characters(line + strlen(table[i])));
		}
		i++;
	}
	return (false);
}

bool	input_is_valid(int argc, char **argv, t_data *data)
{
	char	*line;

	int fd = open("test.rt", O_RDONLY);
	while (true)
	{
		line = get_next_line(fd);
		if (line == NULL)
			return (true);
		if (line[0] == '\0')
		{
			free(line);
			return (true);
		}
		if (line[0] == '#' || line[0] == '\n')
		{
			free(line);
			continue ;
		}
		if (valid_identifier(line) == false)
		{
			printf("Error: invalid identifier\n");
			free(line);
			exit(0);
		}
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (true);
}

bool	convert_input_to_scene(int argc, char **argv, t_data *data)
{
	int		i;
	char	*line;
	char	**split;

	int fd = open("test.rt", O_RDONLY);
	while (true)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (line[0] == '\0')
		{
			free(line);
			return (true);
		}
		if (line[0] == '#' || line[0] == '\n')
		{
			free(line);
			continue ;
		}
		i = 0;
		while (line[i])
		{
			if (line[i] == '\t')
				line[i] = ' ';
			i++;
		}
		split = ft_split(line, ' ');
		if (strcmp(split[0], "A") == 0) //! Ambient light
		{
			if (ft_2d_strlen(split) != 3 || char_count(split[2], ',') != 2 || ft_strlen(split[2]) > 12)
				return (printf("Invalid input: Ambient light\n"), false);
			data->scene.ambient.strength = string_to_float(split[1]);
			if (srgb_to_vec3(split[2], &data->scene.ambient.color) == false)
				return (printf("Invalid input: Ambient light!\n"), false);
			data->scene.ambient.enabled = true;
			free_2d_arr(split);
		}
		else if (strcmp(split[0], "C") == 0) //! Camera
		{
			if (ft_2d_strlen(split) != 4)
				return (printf("Invalid input: Camera\n"), false);
			if (parse_get_pos(split[1], &data->scene.camera.position) == false)
				return (printf("Invalid input: Camera\n"), free_2d_arr(split), false);
			if (parse_get_normal(split[2], &data->scene.camera.direction) == false)
				return (printf("Invalid input: Camera\n"), free_2d_arr(split), false);
			data->scene.camera.vertical_fov = string_to_float(split[3]);
			free_2d_arr(split);
		}
		else if (strcmp(split[0], "sp") == 0) //! Sphere
		{
			if (ft_2d_strlen(split) != 4)
				return (printf("Invalid input: Sphere\n"), false);
			t_vec3 pos;
			if (parse_get_pos(split[1], &pos) == false)
				return (printf("Invalid input: Sphere\n"), free_2d_arr(split), false);
			float radius = string_to_float(split[2]);
			t_vec3 c;
			if (srgb_to_vec3(split[3], &c) == false)
				return (printf("Invalid input: Sphere!\n"), false);
			printf("pos = %f, %f, %f\n", pos.x, pos.y, pos.z);
			printf("radius = %f\n", radius);
			printf("color = %f, %f, %f\n", c.x, c.y, c.z);
		}
		free(line);
	}
	close(fd);
	return (true);
}

bool	parse_input(int argc, char **argv, t_data *data)
{
	
	data->scene.ambient.enabled = false;
	if (input_is_valid(argc, argv, data) == false)
	{
		printf("Error: invalid input\n");
		exit(0);
	}
	printf("parse_input\n");
	convert_input_to_scene(argc, argv, data);
	printf("----------------------\n");


	t_sphere	sphere;
	vec_init(&data->scene.spheres, 16, sizeof(t_sphere));
	vec_init(&data->scene.planes, 16, sizeof(t_plane));
	sphere = create_sphere(vec3_new(-1.5f, 1.5f, 0.0f), 0.4f);
	array_push(&data->scene.spheres, &sphere);


	// printf("ambient = %d\n", data->scene.ambient.enabled);
	// t_vec3 ambient = data->scene.ambient.color;
	// printf("ambient = %f\n", data->scene.ambient.strength);
	// printf("ambient = %f, %f, %f\n", ambient.x, ambient.y, ambient.z);

	// t_vec3 pos = data->scene.camera.position;
	// t_vec3 dir = data->scene.camera.direction;
	// printf("\ncamera = %f, %f, %f\n", pos.x, pos.y, pos.z);
	// printf("camera = %f, %f, %f\n", dir.x, dir.y, dir.z);
	// printf("camera = %f\n", data->scene.camera.vertical_fov);


	// exit(0);
}