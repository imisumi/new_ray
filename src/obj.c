/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 14:31:38 by imisumi           #+#    #+#             */
/*   Updated: 2023/10/24 23:00:38 by imisumi-wsl      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"

void	load_obj_file_data(char *filename, t_vec3 **vertex, t_face **faces, t_vec3 **vn)
{
	int		i;
	int		fd;
	char	*line;

	printf("load_obj_file_data\n");
	fd = open(filename, O_RDONLY);
	i = 0;
	// vec_init(vertex, 16, sizeof(t_vec3));
	// vec_init(faces, 16, sizeof(t_face));
	while (1)
	{
		line = get_next_line(fd);
		if (!line || line[0] == '\0')
			break ;
		if (!strncmp(line, "v ", 2))
		{
			t_vec3	vert;
			sscanf(line, "v %f %f %f", &vert.x, &vert.y, &vert.z);
			// array_push(mesh.vertices, vertex);
			array_push(vertex, &vert);
			
			// printf("%s", line);
		}
		if (!strncmp(line, "f ", 2))
		{
			int	vertex_indices[3];
			int	textrure_indices[3];
			int	normal_indices[3];
			sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
			&vertex_indices[0], &textrure_indices[0], &normal_indices[0],
			&vertex_indices[1], &textrure_indices[1], &normal_indices[1],
			&vertex_indices[2], &textrure_indices[2], &normal_indices[2]
			);
			// t_face	face = {
			// 	vertex_indices[0],
			// 	vertex_indices[1],
			// 	vertex_indices[2]
			// };
			t_face face;
			face.index[0] = vertex_indices[0];
			face.index[1] = vertex_indices[1];
			face.index[2] = vertex_indices[2];
			array_push(faces, &face);

			t_vec3 normal;
			normal.x = normal_indices[0];
			normal.y = normal_indices[1];
			normal.z = normal_indices[2];
			array_push(vn, &normal);
			
			// printf("%s", line);
		}
		free(line);
	}
}