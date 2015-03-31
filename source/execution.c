/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/22 17:15:13 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/31 17:49:54 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh1.h"
#include "ft_sh1_prototypes.h"

// void		redirections_out(t_pipe *pipe_list, int *fd_tab, int j)
// {
// 	int			fd_file;

// 	if (pipe_list->flag_1)
// 	{
// 		fd_file = open(pipe_list->file_for_out, O_TRUNC | O_CREAT | O_RDWR, ACCESSPERMS);
// 		dup2(fd_file, STDOUT);
// 		close(fd_file);
// 	}
// 	else if (pipe_list->flag_2)
// 	{
// 		fd_file = open(pipe_list->file_for_out, O_CREAT | O_APPEND | O_RDWR, ACCESSPERMS);
// 		dup2(fd_file, STDOUT);
// 		close(fd_file);
// 	}
// 	else if (pipe_list->next)
// 		dup2(fd_tab[j + 1], STDOUT);
// }

// void		redirections_in(t_pipe *pipe_list, int *fd_tab, int j, int nbr_pipes)
// {
// 	int			fd_file;

// 	if (pipe_list->flag_3)
// 	{
// 		if ((fd_file = open(pipe_list->file_for_in, O_RDWR)) == -1)
// 		{
// 			err_msg(pipe_list->file_for_in);
// 			err_msg(": No such file or directory.\n");
// 			exit(0);
// 		}
// 		dup2(fd_file, STDIN);
// 		close(fd_file);
// 	}
// 	else if (pipe_list->flag_4)
// 		redir_here_document(pipe_list->file_for_in, fd_tab, nbr_pipes);
// 	else if (pipe_list->num != 0)
// 		dup2(fd_tab[j - 2], STDIN);
// }

void		execute_command(char **cmd, char **env, t_res *res)
{
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
	free(path);
}

void		execute_pipe_lists(t_pipe *pipe_list, int nbr_pipes, char ***env, t_res **res)
{
	int			*fd_tab;
	int			j;

	fd_tab = create_fd_tab(nbr_pipes);
	j = 0;
	while (pipe_list)
	{
		if (is_builtin(pipe_list->cmd_tab) && pipe_list->num == nbr_pipes && check_flags(pipe_list) == 0)
		{
			execute_builtin(pipe_list, env, res, 0);
		}
		else if (fork() == 0)
		{
			redirections_files(pipe_list, fd_tab, nbr_pipes);
			redirections_pipes(pipe_list, fd_tab, j);
			close_fd_tab(fd_tab, nbr_pipes);
			if (is_builtin(pipe_list->cmd_tab) == 1)
				execute_builtin(pipe_list, env, res, nbr_pipes);
			else
				execute_command(pipe_list->cmd_tab, *env, *res);
		}
		pipe_list = pipe_list->next;
		j += 2;
	}
	close_fd_tab(fd_tab, nbr_pipes);
	wait_children(nbr_pipes);
}

int			execute_gen_lists(t_general *gen_list, char ***env, t_res **res)
{
	while (gen_list)
	{
		execute_pipe_lists(gen_list->pipe_list, gen_list->nbr_pipes, env, res);
		gen_list = gen_list->next;
	}
	return (0);
}

