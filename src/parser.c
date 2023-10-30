/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 02:06:12 by ichiro            #+#    #+#             */
/*   Updated: 2023/10/30 03:59:29 by imisumi-wsl      ###   ########.fr       */
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
			exit(0);
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
		if (strcmp(split[0], "A") == 0)
		{
			if (ft_2d_strlen(split) != 3 || char_count(split[2], ',') != 2 || ft_strlen(split[2]) > 12)
				return (printf("Invalid input: Ambient light\n"), false);
			float f = string_to_float(split[1]);
			printf("f: %f\n", f);
			t_vec3 color;
			if (srgb_to_vec3(split[2], &color) == false)
				return (printf("Invalid input: Ambient light!\n"), false);
			printf("color: %f, %f, %f\n", color.x, color.y, color.z);
		}
		// for (int i = 0; split[i]; i++)
		// {
		// 	// if (ft_strchr(split[i], '.'))
		// 	// {
		// 	// 	float f = string_to_float(split[i]);
		// 	// 	printf("f: %f\n", f);
		// 	// }
		// 	// printf("%s\n", split[i]);
		// 	// if (strcmp(split[i], "A") == 0)
		// 	// {
		// 	// 	// if (split[3][0] != '\0')
		// 	// 	// 	printf("Error: invalid input\n");
		// 	// }
		// }
		free(line);
	}
	close(fd);
	return (true);
}

bool	parse_input(int argc, char **argv, t_data *data)
{
	if (input_is_valid(argc, argv, data) == false)
	{
		printf("Error: invalid input\n");
		exit(0);
	}
	printf("parse_input\n");
	convert_input_to_scene(argc, argv, data);


	exit(0);
}