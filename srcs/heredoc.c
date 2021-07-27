/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmayweat <gmayweat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 06:10:38 by gmayweat          #+#    #+#             */
/*   Updated: 2021/07/27 11:33:25 by gmayweat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	heredoc(t_pipex *all)
{
	char	buf[100];
	int		fd;
	int		f;

	f = 0;
	fd = open(".heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd == -1)
		return (all->err = -1);
	while (1)
	{
		ft_bzero(buf, 100);
		write(1, "> ", 2);
		f = read(0, buf, 99);
		if (!ft_strncmp(buf, all->heredoc, ft_strlen(all->heredoc)))
		{
			close(fd);
			return (0);
		}
		write(fd, buf, ft_strlen(buf));
	}
}

int	heredoc_way(t_pipex *all, int argc, char **argv)
{
	all->is_heredoc = 1;
	if (argc < 6)
		return (all->err = 1);
	all->heredoc = ft_strjoin(argv[2], "\n");
	all->fd1 = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (all->fd1 == -1)
		return (all->err = -1);
	if (heredoc(all))
		return (all->err = -1);
	all->fd0 = open(".heredoc", O_RDONLY);
	if (all->fd0 == -1)
		return (all->err = -1);
	return (0);
}
