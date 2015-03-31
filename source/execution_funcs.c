/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/30 14:37:30 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/31 11:51:47 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh1.h"
#include "ft_sh1_prototypes.h"

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

void		examine_status(int status)
{
	if (WTERMSIG(status) == 11)
		err_msg("Segmentation fault\n");
	if (WTERMSIG(status) == 10)
		err_msg("Bus error\n");
}

void		wait_children(int nbr_pipes)
{
	int			status;

	while (nbr_pipes >= 0)
	{
		waitpid(0, &status, WUNTRACED);
		examine_status(status);
		--nbr_pipes;
	}
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
