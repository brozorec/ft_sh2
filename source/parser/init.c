/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/27 17:49:39 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/30 14:52:49 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh2.h"
#include "ft_sh2_prototypes.h"

t_general	*gen_init(void)
{
	t_general			*gen_list;

	gen_list = (t_general *)malloc(sizeof(t_general));
	gen_list->gen_line = 0;
	gen_list->nbr_pipes = 0;
	gen_list->pipe_list = 0;
	gen_list->next = 0;
	return (gen_list);
}

t_pipe		*pipe_init(void)
{
	t_pipe				*pipe_list;

	pipe_list = (t_pipe *)malloc(sizeof(t_pipe));
	pipe_list->num = 0;
	pipe_list->pipe_line = 0;
	pipe_list->cmd_tab = 0;
	pipe_list->file_for_out = 0;
	pipe_list->file_for_in = 0;
	pipe_list->no_name = 0;
	pipe_list->flag_1 = 0;
	pipe_list->flag_2 = 0;
	pipe_list->flag_3 = 0;
	pipe_list->flag_4 = 0;
	pipe_list->next = 0;
	return (pipe_list);
}
