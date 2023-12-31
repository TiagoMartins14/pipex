/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: patatoss <patatoss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 11:18:27 by tiaferna          #+#    #+#             */
/*   Updated: 2023/11/27 23:31:26 by patatoss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	execute_in(char **argv, char **path_array, char **envp, int index)
{
	char	*path;
	char	**cmds;

	cmds = ft_split(argv[index], ' ');
	path = check_path(cmds[0], path_array);
	execve(path, cmds, envp);
	ft_free_array(path_array);
	ft_perror_exit("Error execve.\n", COMMAND_NOT_FOUND);
}

void	execute_out(char **argv, char **path_array, char **envp, int i)
{
	int		fd_out;
	char	*path;
	char	**cmds;

	cmds = ft_split(argv[i++], ' ');
	path = check_path(cmds[0], path_array);
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
		fd_out = open(argv[i], O_RDWR | O_APPEND | O_CREAT, 0644);
	else
		fd_out = open(argv[i], O_RDWR | O_TRUNC | O_CREAT, 0644);
	if (fd_out == -1)
	{
		ft_free_array(path_array);
		ft_perror_exit("Error fd_out\n", 1);
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	execve(path, cmds, envp);
	ft_free_array(path_array);
	ft_perror_exit("Error.\n", COMMAND_NOT_FOUND);
}
