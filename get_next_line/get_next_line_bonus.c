/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akaterji <akaterji@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 18:36:55 by akaterji          #+#    #+#             */
/*   Updated: 2024/06/21 14:52:54 by akaterji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	get_len(char *str)
{
	size_t		len;

	len = 0;
	if (str == NULL)
		return (0);
	while (str[len] != 0)
		len++;
	return (len);
}

char	*free_and_null(char *buffer)
{
	free(buffer);
	return (NULL);
}

char	*get_static_line(char *sline, int fd)
{
	char	*buffer;
	int		read_bytes;
	int		flag_new;

	flag_new = 0;
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	read_bytes = read(fd, buffer, BUFFER_SIZE);
	if (read_bytes == -1 || (!buffer && !sline))
		return (free_and_null(buffer));
	buffer[read_bytes] = '\0';
	while (read_bytes > 0 && !flag_new)
	{
		sline = update_static_line(sline, buffer, &flag_new, read_bytes);
		if (!flag_new)
		{
			read_bytes = read(fd, buffer, BUFFER_SIZE);
			if (read_bytes == -1)
				return (free_and_null(buffer));
			buffer[read_bytes] = '\0';
		}
	}
	free(buffer);
	return (sline);
}

char	*get_next_line(int fd)
{
	static char	*sline[1048];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	sline[fd] = get_static_line(sline[fd], fd);
	if (sline[fd] == NULL)
		return (NULL);
	line = get_target_line(sline[fd]);
	sline[fd] = ft_remove_line(sline[fd]);
	return (line);
}
int	main(void)
{
	char	*line;
	int		i;
	int		fd1;
	int		fd2;
	int		fd3;

	fd1 = open("test.txt", O_RDONLY);
	fd2 = open("test2.txt", O_RDONLY);
	fd3 = open("test3.txt", O_RDONLY);
	i = 1;
	while (i < 7)
	{
		line = get_next_line(fd1);
		printf("line [%02d]: %s", i, line);
		free(line);
		line = get_next_line(fd2);
		printf("line [%02d]: %s", i, line);
		free(line);
		line = get_next_line(fd3);
		printf("line [%02d]: %s", i, line);
		free(line);
		i++;
	}
	close(fd1);
	close(fd2);
	close(fd3);
	return (0);
}