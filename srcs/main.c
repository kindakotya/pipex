/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmayweat <gmayweat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 23:44:08 by gmayweat          #+#    #+#             */
/*   Updated: 2021/07/27 08:30:10 by gmayweat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	free_char_z_z(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
	{
		if (arr[i])
			free(arr[i]);
		++i;
	}
	if (arr)
		free(arr);
}

static void	pipex_free(t_pipex *all)
{
	int	i;

	if (all->is_heredoc)
		unlink(".heredoc");
	i = 0;
	while (all->argvs && all->argvs[i])
	{
		free_char_z_z(all->argvs[i]);
		++i;
	}
	if (all->argvs)
		free(all->argvs);
	free_char_z_z(all->path);
}

int	pipex_err(t_pipex *all, int f)
{
	if (!all || !all->err)
	{
		if (f == 1)
			ft_putstr_fd("Error: wrong number of arguments.\n", 2);
		if (f == 2)
			ft_putstr_fd("Error: there is no PATH in env.\n", 2);
		if (all && f >= 0)
			all->err = f;
		else if (all)
			perror("pipex");
		if (all)
			pipex_free(all);
	}
	return (f);
}

static int	standart_way(t_pipex *all, char **argv)
{
	all->fd0 = open(argv[1], O_RDONLY);
	if (all->fd0 == -1)
		return (pipex_err(0, -1));
	all->fd1 = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (all->fd1 == -1)
		return (pipex_err(all, -1));
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	all;

	ft_bzero(&all, sizeof(t_pipex));
	if (argc < 5)
		return (pipex_err(0, 1));
	if (ft_strncmp(argv[1], "here_doc", 9))
	{
		if (heredoc_way(&all, argc, argv))
			return (all.err);
	}
	else
		if (standart_way(&all, argv))
			return (all.err);
	if (!cmd_alloc(&all, argc, argv, env))
		pipex(&all, env);
	pipex_free(&all);
	return (0);
}
