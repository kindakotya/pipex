/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmayweat <gmayweat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 04:18:31 by gmayweat          #+#    #+#             */
/*   Updated: 2021/07/27 10:49:58 by gmayweat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	check_in_path(t_pipex *all, int i)
{
	char	*meow;
	char	*cmd;
	int		j;

	j = 0;
	meow = ft_strjoin("/", all->argvs[i][0]);
	if (!meow)
		return (all->err = -1);
	while (all->path[j])
	{
		cmd = ft_strjoin(all->path[j], meow);
		if (!access(cmd, 0))
		{
			free(all->argvs[i][0]);
			all->argvs[i][0] = cmd;
			free(meow);
			return (0);
		}
		free(cmd);
		cmd = 0;
		++j;
	}
	free(meow);
	free(cmd);
	return (all->err = 1);
}

static int	cmd_check(t_pipex *all)
{
	int		i;

	i = 0;
	while (i < all->nu_comm)
	{
		if (!ft_strncmp(all->argvs[i][0], "./", 2) && all->argvs[i][0][3] != 0)
		{
			if (access(all->argvs[i][0], 0))
				return (all->err = -1);
			continue ;
		}
		else
		{
			if (check_in_path(all, i))
				return (all->err = -1);
		}
		++i;
	}
	return (0);
}

static int	get_path(t_pipex *all, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
			break ;
		++i;
	}
	if (!env[i])
		return (all->err = 2);
	all->path = ft_split(env[i] + 5, ':');
	if (!all->path)
		return (all->err = -1);
	return (0);
}

int	cmd_alloc(t_pipex *all, int argc, char **argv, char **env)
{
	int	i;

	if (get_path(all, env))
		return (all->err = -1);
	all->nu_comm = argc - 3 - all->is_heredoc;
	all->argvs = ft_calloc(all->nu_comm + 1, sizeof(char **));
	if (!all->argvs)
		return (all->err = -1);
	i = 0;
	while (i < all->nu_comm)
	{
		all->argvs[i] = ft_split(argv[i + 2 + all->is_heredoc], ' ');
		if (!all->argvs[i])
			return (all->err = -1);
		++i;
	}
	all->pipes_fd = ft_calloc(all->nu_comm, sizeof(int *));
	i = 0;
	while (i < all->nu_comm)
		all->pipes_fd[i++] = ft_calloc(2, sizeof(int));
	if (cmd_check(all))
		return (all->err = -1);
	return (0);
}
