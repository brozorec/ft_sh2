/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_funcs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/30 14:37:30 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/30 14:37:31 by bbarakov         ###   ########.fr       */
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

void		wait_children(int nbr_pipes)
{
	while (nbr_pipes >= 0)
	{
		wait(0);
		--nbr_pipes;
	}
}
