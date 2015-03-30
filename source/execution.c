/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/22 17:15:13 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/30 20:08:28 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh1.h"
#include "ft_sh1_prototypes.h"

void		redir_here_doc(t_pipe *pipe_list, int *fd_tab, int j)
{
	char		*buff;
	int			len;
	// int			fd_new_tab[2];

	buff = 0;
	// pipe(fd_new_tab);
	// if (fork() == 0)
	// {
	// 	close(fd_new_tab[READ_END]);
	while (get_next_line(0, &buff) != 0 && ft_strcmp(buff, pipe_list->file_for_in))
	{
		len = ft_strlen(buff);
		buff[len] = '\n';
		write(fd_tab[j], buff, len + 1);
	}
		// close(fd_new_tab[WRITE_END]);
		// exit(0);
	// }
	// wait(0);
	// close(fd_new_tab[WRITE_END]);
	// dup2(fd_new_tab[READ_END], fd_tab[j + 2]);
	// close(fd_new_tab[READ_END]);
}

void		examine_status(int status, pid_t child)
{
	if (WIFEXITED(status))
		ft_putstr("@>");
	else if (WIFSTOPPED(status))
		kill(child, 9);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) != 2 && WTERMSIG(status) != 3 &&
			WTERMSIG(status) != 18 && WTERMSIG(status) != 10 &&
			WTERMSIG(status) != 11)
			ft_putstr("@>");
		if (WTERMSIG(status) == 10)
		{
			err_msg("Bus error\n");
			ft_putstr("@>");
		}
		if (WTERMSIG(status) == 11)
		{
			err_msg("Segmentation fault\n");
			ft_putstr("@>");
		}
	}
}

void		redir_here_document(t_pipe *pipe_list, int *fd_tab, int j, int nbr_pipes)
{
	char		*buff;
	int			len;
	int			fd_new_tab[2];

	buff = 0;
	pipe(fd_new_tab);
	if (fork() == 0)
	{
		close_fd_tab(fd_tab, nbr_pipes);
		close(fd_new_tab[READ_END]);
		while (get_next_line(0, &buff) != 0 && ft_strcmp(buff, pipe_list->file_for_in))
		{
			ft_putstr_fd("?", 2);
			len = ft_strlen(buff);
			buff[len] = '\n';
			write(fd_new_tab[WRITE_END], buff, len + 1);
		}
		close(fd_new_tab[WRITE_END]);
		exit(0);
	}
	wait(0);
	close(fd_new_tab[WRITE_END]);
	// if (pipe_list->next)
	// 	dup2(fd_new_tab[READ_END], fd_tab[j + 2]);
	// else
	dup2(fd_new_tab[READ_END], fd_tab[j + 2]);
	close(fd_new_tab[READ_END]);
	// close(fd_tab[j + 1]);
}

void		redirections_out(t_pipe *pipe_list, int *fd_tab, int j)
{
	int			fd_file;

	if (pipe_list->flag_1)
	{
		fd_file = open(pipe_list->file_for_out, O_TRUNC | O_CREAT | O_RDWR, ACCESSPERMS);
		dup2(fd_file, STDOUT);
		close(fd_file);
	}
	else if (pipe_list->flag_2)
	{
		fd_file = open(pipe_list->file_for_out, O_CREAT | O_APPEND | O_RDWR, ACCESSPERMS);
		dup2(fd_file, STDOUT);
		close(fd_file);
	}
	else if (pipe_list->next && pipe_list->flag_4 == 0)
		dup2(fd_tab[j + 1], STDOUT);
}

void		redirections_in(t_pipe *pipe_list, int *fd_tab, int j, int nbr_pipes)
{
	int			fd_file;

	if (pipe_list->flag_3)
	{
		if ((fd_file = open(pipe_list->file_for_in, O_RDWR)) == -1)
		{
			err_msg(pipe_list->file_for_in);
			err_msg(": No such file or directory.\n");
			exit(0);
		}
		dup2(fd_file, STDIN);
		close(fd_file);
	}
	else if (pipe_list->flag_4)
	{
		// dup2(fd_tab[j + 2], fd_tab[j + 1]);
		redir_here_document(pipe_list, fd_tab, j, nbr_pipes);
	}
	else if (pipe_list->num != 0)
		dup2(fd_tab[j - 2], STDIN);
}

void		execute_pipe_lists(t_pipe *pipe_list, int nbr_pipes, char ***env, t_res **res)
{
	int			*fd_tab;
	int			j;

	fd_tab = create_fd_tab(nbr_pipes);
	j = 0;
	while (pipe_list)
	{
		if (fork() == 0)
		{
			redirections_in(pipe_list, fd_tab, j, nbr_pipes);
			redirections_out(pipe_list, fd_tab, j);
			close_fd_tab(fd_tab, nbr_pipes);
			if (is_builtin(pipe_list->cmd_tab) == 1 && pipe_list->num == nbr_pipes)
				execute_builtin(pipe_list->cmd_tab, env, res);
			else
				execute_command(pipe_list->cmd_tab, *env, *res);
		}
		pipe_list = pipe_list->next;
		j += 2;
	}
	close_fd_tab(fd_tab, nbr_pipes);
	wait_children(nbr_pipes);
}

char		*path_to_exec(char **cmd, char **env, t_res *res)
{
	char		*path;

	path = 0;
	if (cmd[0][0] == '/' || cmd[0][0] == '.')
		path = ft_strdup(cmd[0]);
	else if ((path = lookup_paths("PATH=", cmd[0], env)) == 0
		&& (path = lookup_paths("PATH=", cmd[0], res->paths)) == 0)
	{
		err_msg(cmd[0]);
		err_msg(": Command not found.\n");
		exit(0);
	}
	return (path);
}

void		execute_command(char **cmd, char **env, t_res *res)
{
	// pid_t		child;
	// int			status;
	char		*path;

	if ((path = path_to_exec(cmd, env, res)) == 0)
		return ;
	if (execve(path, cmd, env) == -1)
	{
		err_msg(path);
		free(path);
		err_msg(": Permission denied.\n");
		exit(127);
	}
	// waitpid(child, &status, WUNTRACED);
	// examine_status(status, child);
	free(path);
}
