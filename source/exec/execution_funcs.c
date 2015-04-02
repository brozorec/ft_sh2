/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/30 14:37:30 by bbarakov          #+#    #+#             */
/*   Updated: 2015/04/02 15:25:20 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh2.h"
#include "ft_sh2_prototypes.h"

void		close_fd_tab(int *fd_tab, int nbr_pipes)
{
	int			i;

	i = 0;
	while (i < 2 * nbr_pipes)
	{
		close(fd_tab[i]);
		++i;
	}
}

int			*create_fd_tab(int nbr_pipes)
{
	int			i;
	int			*fd_tab;

	i = 0;
	fd_tab = (int *)malloc(sizeof(int) * (2 * nbr_pipes));
	while (i < nbr_pipes)
	{
		pipe(fd_tab + (i * 2));
		++i;
	}
	return (fd_tab);
}

void		examine_status(int status, pid_t pid)
{
	if (WTERMSIG(status) == 11)
		err_msg("Segmentation fault\n");
	if (WTERMSIG(status) == 10)
		err_msg("Bus error\n");
	if (WIFSTOPPED(status))
	{
		kill(pid, 9);
		ft_putstr_fd(
			"Job control not supported. Suspended proccess killed.\n", 2);
	}
}

void		wait_children(int nbr_pipes)
{
	int			status;
	pid_t		pid;

	while (nbr_pipes >= 0)
	{
		pid = waitpid(0, &status, WUNTRACED);
		examine_status(status, pid);
		--nbr_pipes;
	}
}

char		*path_to_exec(char **cmd, char **env, t_res *res)
{
	char		*path;

	path = 0;
	if (cmd[0][0] == '/' || cmd[0][0] == '.')
		path = ft_strdup(cmd[0]);
	else if ((path = lookup_paths("PATH=", cmd[0], env)) == 0)
		path = lookup_paths("PATH=", cmd[0], res->paths);
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
			return (path);
		else
		{
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": permission denied.\n", 2);
			exit(0);
		}
	}
	else
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found.\n", 2);
		exit(0);
	}
	return (0);
}
