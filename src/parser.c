/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 02:06:12 by ichiro            #+#    #+#             */
/*   Updated: 2023/10/30 15:59:59 by imisumi          ###   ########.fr       */
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
		}
		else if (strcmp(split[0], "C") == 0) //! Camera
		{
			if (ft_2d_strlen(split) != 4)
				return (printf("Invalid input: Camera\n"), false);
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
	printf("ambient = %d\n", data->scene.ambient.enabled);
	t_vec3 ambient = data->scene.ambient.color;
	printf("ambient = %f\n", data->scene.ambient.strength);
	printf("ambient = %f, %f, %f\n", ambient.x, ambient.y, ambient.z);


	exit(0);
}