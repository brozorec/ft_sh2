/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/22 17:15:13 by bbarakov          #+#    #+#             */
/*   Updated: 2015/04/02 14:56:50 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh2.h"
#include "ft_sh2_prototypes.h"

void
	execute_command(char **cmd, char **env, t_res *res)
{
	char		*path;

	if ((path = path_to_exec(cmd, env, res)) == 0)
		return ;
	if (execve(path, cmd, env) == -1)
	{
		exit(127);
	}
}

void
	execute_pipe_lst(t_pipe *pipe_list, int nbr_pipes, char ***env, t_res **res)
{
	int			*fd_tab;
	int			j;

	fd_tab = create_fd_tab(nbr_pipes);
	j = 0;
	while (pipe_list)
	{
		if (is_builtin(pipe_list->cmd_tab) && pipe_list->num == nbr_pipes
			&& check_flags(pipe_list, nbr_pipes) == 0)
			execute_builtin(pipe_list, env, res, 0);
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

int
	execute_gen_lists(t_general *gen_list, char ***env, t_res **res)
{
	while (gen_list)
	{
		execute_pipe_lst(gen_list->pipe_list, gen_list->nbr_pipes, env, res);
		gen_list = gen_list->next;
	}
	return (0);
}
