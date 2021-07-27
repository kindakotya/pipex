/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmayweat <gmayweat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 04:59:44 by gmayweat          #+#    #+#             */
/*   Updated: 2021/07/27 11:23:18 by gmayweat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	last_pipe(t_pipex *all, char **env, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		dup2(all->pipes_fd[i - 1][0], 0);
		close(all->pipes_fd[i - 1][0]);
		dup2(all->fd1, 1);
		close(all->fd1);
		execve(all->argvs[i][0], all->argvs[i], env);
	}
	wait(0);
	close(all->pipes_fd[i - 1][0]);
	close(all->fd1);
	return (0);
}

int	other_pipes(t_pipex *all, char **env, int i)
{
	pid_t	pid;

	pipe(all->pipes_fd[i]);
	pid = fork();
	if (pid == 0)
	{
		dup2(all->pipes_fd[i - 1][0], 0);
		close(all->pipes_fd[i - 1][0]);
		dup2(all->pipes_fd[i][1], 1);
		close(all->pipes_fd[i][1]);
		if (execve(all->argvs[i][0], all->argvs[i], env))
			return (all->err = -1);
	}
	wait(0);
	close(all->pipes_fd[i - 1][0]);
	close(all->pipes_fd[i][1]);
	return (0);
}

int	pipex(t_pipex *all, char **env)
{
	pid_t	pid;
	int		i;

	pipe(all->pipes_fd[0]);
	pid = fork();
	if (pid == 0)
	{
		dup2(all->fd0, 0);
		close(all->fd0);
		dup2(all->pipes_fd[0][1], 1);
		close(all->pipes_fd[0][1]);
		if (execve(all->argvs[0][0], all->argvs[0], env) == -1)
			return (all->err = -1);
	}
	wait(0);
	close(all->pipes_fd[0][1]);
	close(all->fd0);
	i = 1;
	while (i < all->nu_comm - 1 && all->argvs[i + 1])
		if (other_pipes(all, env, i++))
			return (all->err = -1);
	if (!last_pipe(all, env, i))
		return (all->err = -1);
	return (0);
}
