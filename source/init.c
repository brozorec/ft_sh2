/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/27 17:49:39 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/27 17:49:40 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh1.h"
#include "ft_sh1_prototypes.h"

t_cmd		*cmd_init(void)
{
	t_cmd				*cmd_list;

	cmd_list = (t_cmd *)malloc(sizeof(t_cmd));
	cmd_list->cmd_line = 0;
	cmd_list->pipe_list = 0;
	cmd_list->next = 0;
	return (cmd_list);
}

t_pipe		*pipe_init(void)
{
	t_pipe				*pipe_list;

	pipe_list = (t_pipe *)malloc(sizeof(t_pipe));
	pipe_list->tab = 0;
	pipe_list->next = 0;
	return (pipe_list);
}
