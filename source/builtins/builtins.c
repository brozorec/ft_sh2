/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/21 19:22:17 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/31 17:53:17 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh2.h"
#include "ft_sh2_prototypes.h"

int
	is_builtin(char **cmd)
{
	if (!ft_strcmp(cmd[0], "cd") || !ft_strcmp(cmd[0], "setenv") ||
		!ft_strcmp(cmd[0], "unsetenv") || !ft_strcmp(cmd[0], "exit") ||
		!ft_strcmp(cmd[0], "env") || !ft_strcmp(cmd[0], "pwd"))
	{
		return (1);
	}
	return (0);
}

void
	execute_builtin(t_pipe *pipe_list, char ***env, t_res **res, int nbr_pipes)
{
	if (!ft_strcmp(pipe_list->cmd_tab[0], "cd"))
		cd_builtin(pipe_list->cmd_tab, env, res);
	else if (!ft_strcmp(pipe_list->cmd_tab[0], "exit"))
		exit_builtin(pipe_list->cmd_tab, env, *res);
	else if (!ft_strcmp(pipe_list->cmd_tab[0], "env"))
		env_builtin(pipe_list->cmd_tab, *env);
	else if (!ft_strcmp(pipe_list->cmd_tab[0], "setenv"))
		setenv_builtin(pipe_list->cmd_tab, env, res);
	else if (!ft_strcmp(pipe_list->cmd_tab[0], "unsetenv"))
		unsetenv_builtin(pipe_list->cmd_tab, env);
	else if (!ft_strcmp(pipe_list->cmd_tab[0], "pwd"))
		pwd_builtin(*env);
	if (nbr_pipes || check_flags(pipe_list, nbr_pipes) == 1)
		exit(0);
}
