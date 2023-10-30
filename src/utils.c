/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 02:06:12 by ichiro            #+#    #+#             */
/*   Updated: 2023/10/30 03:40:59 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"

void	free_2d_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

uint64_t	simple_string_to_int(char *str, int *i)
{
	uint64_t	res;

	res = 0;
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		res = res * 10 + (str[*i] - '0');
		(*i)++;
	}
	return (res);
}

float	string_to_float(char *str)
{
	int	i;
	int	sign;
	float	res;
	uint64_t	start;
	uint64_t	end;

	i = 0;
	sign = 1;
	if (str[i] == '-')
	{
		i++;
		sign = -1;
	}
	start = simple_string_to_int(str, &i);
	if (str[i] == '\0')
		return (start * sign);
	if (str[i] != '.')
		return (NAN);
	i++;
	end = simple_string_to_int(str, &i);
	if (end == 0)
		return (start * sign);
	int	num_digits = pow(10, floor(log10(end)) + 1);
	res = start + (float)end / num_digits;
	return (res * sign);
}

// float string_to_float(char *str) {
//     char *endptr;
//     float result = strtof(str, &endptr);
//     if (*endptr != '\0' && *endptr != '\n') {
//         // Conversion failed, handle error if necessary
//         printf("Error: Invalid input string\n");
//         exit(EXIT_FAILURE);
//     }
//     return result;
// }

// returns the number of a specific character in a string
int	char_count(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

static bool	str_num_only(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

//! positive number only
static int64_t	ft_atoi2(char *str)
{
	int			i;
	uint64_t	num;

	if (str == NULL)
		return (-1);
	if (str_num_only(str) == false)
		return (-1);
	i = 0;
	num = 0;
	while (str[i])
	{
		num = num * 10 + str[i] - 48;
		i++;
	}
	if (num > 255)
		return (-1);
	return (num);
}

int	ft_2d_strlen(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

bool	srgb_to_vec3(char *str, t_vec3 *color)
{
	int		r;
	int		g;
	int		b;
	char	**split;
	if (str[ft_strlen(str) - 1] == '\n')
		str[ft_strlen(str) - 1] = '\0';
	split = ft_split(str, ',');
	if (split == NULL)
		return (false);
	if (ft_2d_strlen(split) != 3)
		return (free_2d_arr(split), false);
	r = ft_atoi2(split[0]);
	g = ft_atoi2(split[1]);
	b = ft_atoi2(split[2]);
	if (r == -1 || g == -1 || b == -1)
		return (free_2d_arr(split), false);
	color->x = r / 255.0f;
	color->y = g / 255.0f;
	color->z = b / 255.0f;
	return (free_2d_arr(split), true);
}


