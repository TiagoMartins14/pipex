/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiaferna <tiaferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 11:18:27 by tiaferna          #+#    #+#             */
/*   Updated: 2023/11/29 07:47:50 by tiaferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	open_file(char *infile)
{
	int	fd;

	if (access(infile, F_OK | R_OK) == 0)
	{
		fd = open(infile, O_RDONLY);
		if (fd == -1)
			ft_perror_exit("Infile error.\n", 1);
	}
	else
	{
		if (access(infile, F_OK) == -1 || access(infile, R_OK) == -1)
			ft_perror_exit("Infile error.\n", 1);
		fd = open("/dev/null", O_RDONLY);
		if (fd == -1)
			ft_perror_exit("Infile error.\n", 1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	pipex(char **argv, char **path_array, char **envp, int i)
{
	int	fd_pipe[2];
	int	pid;

	if (pipe(fd_pipe) == -1)
		free_array_and_exit(path_array, "Pipe error.\n");
	pid = fork();
	if (pid == -1)
		free_array_and_exit(path_array, "Fork error.\n");
	if (pid == 0)
	{
		close(fd_pipe[READ_END]);
		dup2(fd_pipe[WRITE_END], STDOUT_FILENO);
		close(fd_pipe[WRITE_END]);
		execute_in(argv, path_array, envp, i);
	}
	close(fd_pipe[WRITE_END]);
	waitpid(pid, NULL, 0);
	dup2(fd_pipe[READ_END], STDIN_FILENO);
	close(fd_pipe[READ_END]);
}

int	main(int argc, char **argv, char **envp)
{
	char	**path_array;
	int		i;
	char	*infile;

	command_check(argc, argv);
	infile = ft_check_infile(argv[1]);
	i = 2;
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		get_input(argv[2]);
		i = 3;
	}
	else
		open_file(infile);
	path_array = create_path_array(envp);
	if (!path_array)
		ft_perror_exit("Path not found.", 1);
	while (i < argc - 2)
		pipex(argv, path_array, envp, i++);
	if (!ft_strncmp(argv[1], "/dev/urandom", ft_strlen("/dev/urandom")))
		unlink_and_free(infile);
	execute_out(argv, path_array, envp, i);
	ft_free_array(path_array);
	return (COMMAND_NOT_FOUND);
}
