/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmayweat <gmayweat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 23:45:54 by gmayweat          #+#    #+#             */
/*   Updated: 2021/07/27 08:11:51 by gmayweat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "libft.h"

typedef struct s_pipex
{
	u_int16_t	is_heredoc;
	u_int16_t	nu_comm;
	char		*heredoc;
	char		***argvs;
	char		**path;
	int			fd01[2];
	int			fd0;
	int			fd1;
	int			err;
}			t_pipex;

int	cmd_alloc(t_pipex *all, int argc, char **argv, char **env);
int	heredoc_way(t_pipex *all, int argc, char **argv);
int	pipex(t_pipex *all, char **env);
int	pipex_err(t_pipex *all, int f);
#endif