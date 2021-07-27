/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmayweat <gmayweat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 04:59:44 by gmayweat          #+#    #+#             */
/*   Updated: 2021/07/27 08:11:25 by gmayweat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	last_pipe(t_pipex *all, char **env, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		dup2(all->fd01[0], 0);
		close(all->fd01[0]);
		dup2(all->fd1, 1);
		close(all->fd1);
		if (execve(all->argvs[i][0], all->argvs[i], env))
			return (-1);
	}
	wait(0);
	close(all->fd01[0]);
	close(all->fd1);
	all->fd1 = 0;
	all->fd0 = 0;
	ft_bzero(all->fd01, 2 * sizeof(int));
	return (0);
}

int	other_pipes(t_pipex *all, char **env, int i)
{
	pid_t	pid;
	int		fds[2];

	pipe(fds);
	close(all->fd01[1]);
	pid = fork();
	if (pid == 0)
	{
		close(fds[0]);
		dup2(all->fd01[0], 0);
		close(all->fd01[0]);
		dup2(fds[1], 1);
		close(fds[1]);
		if (execve(all->argvs[i][0], all->argvs[i], env))
			return (-1);
	}
	wait(0);
	close(fds[0]);
	all->fd01[0] = fds[1];
	return (0);
}

int	pipex(t_pipex *all, char **env)
{
	pid_t	pid;
	int		i;

	pipe(all->fd01);
	pid = fork();
	if (pid == 0)
	{
		close(all->fd01[0]);
		dup2(all->fd0, 0);
		close(all->fd0);
		dup2(all->fd01[1], 1);
		close(all->fd01[1]);
		if (execve(all->argvs[0][0], all->argvs[0], env) == -1)
			return (pipex_err(all, -1));
	}
	wait(0);
	i = 1;
	while (all->argvs[i + 1])
		if (other_pipes(all, env, i++))
			return (pipex_err(all, -1));
	if (!last_pipe(all, env, i))
		return (pipex_err(all, -1));
	return (0);
}
