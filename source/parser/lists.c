/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/28 13:20:48 by bbarakov          #+#    #+#             */
/*   Updated: 2015/04/02 13:24:08 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh2.h"
#include "ft_sh2_prototypes.h"

void
	free_gen_list(t_general *gen_list)
{
	t_general			*head_gen;
	t_pipe				*head_pipe;

	while (gen_list)
	{
		while (gen_list->pipe_list)
		{
			free(gen_list->pipe_list->pipe_line);
			ft_str3del(gen_list->pipe_list->cmd_tab);
			free(gen_list->pipe_list->file_for_in);
			free(gen_list->pipe_list->file_for_out);
			head_pipe = gen_list->pipe_list->next;
			free(gen_list->pipe_list);
			gen_list->pipe_list = head_pipe;
		}
		head_gen = gen_list->next;
		free(gen_list);
		gen_list = head_gen;
	}
}

int
	check_flags(t_pipe *p, int nbr_pipes)
{
	if (p->flag_2 > 1 || p->flag_1 > 1 || (p->flag_2 == 1 && p->flag_1 == 1))
		return (2);
	if (p->num != nbr_pipes && (p->flag_1 || p->flag_2))
		return (2);
	if (p->flag_4 > 1 || p->flag_3 > 1 || (p->flag_4 == 1 && p->flag_3 == 1))
		return (3);
	if (p->num > 0 && (p->flag_3 || p->flag_4))
		return (3);
	if (p->flag_1 || p->flag_2 || p->flag_3 || p->flag_4)
		return (1);
	return (0);
}

t_pipe
	*examine_pipe(t_pipe *pipe, int nbr_pipes, char **env, t_res *res)
{
	pipe = get_redirections_output(pipe);
	pipe = get_redirections_input(pipe);
	if (pipe->no_name)
	{
		ft_putstr_fd("Ambiguous or missing name for redirect.\n", 2);
		return (0);
	}
	if (check_flags(pipe, nbr_pipes) == 2)
	{
		ft_putstr_fd("Ambiguous output redirect.\n", 2);
		return (0);
	}
	if (check_flags(pipe, nbr_pipes) == 3)
	{
		ft_putstr_fd("Ambiguous input redirect.\n", 2);
		return (0);
	}
	if ((pipe = get_cmd(pipe, env, res)) == 0)
		return (0);
	return (pipe);
}

t_pipe
	*get_pp(char *gen_line, int nbr_pipes, char **env, t_res *res)
{
	int					i;
	char				**tab;
	t_pipe				*pipe_list;
	t_pipe				*head;

	i = 0;
	tab = ft_strsplit(gen_line, '|');
	pipe_list = pipe_init();
	head = pipe_list;
	while (tab[i])
	{
		pipe_list->pipe_line = ft_strdup(tab[i]);
		pipe_list->num = i;
		if ((pipe_list = examine_pipe(pipe_list, nbr_pipes, env, res)) == 0)
			return (0);
		if (tab[++i])
		{
			pipe_list->next = pipe_init();
			pipe_list = pipe_list->next;
		}
	}
	if (err_null_command(pipe_list, nbr_pipes) == 0)
		return (0);
	ft_str3del(tab);
	return (head);
}

t_general
	*get_gen_lists(char *line, t_general *gen, char **env, t_res *res)
{
	int					i;
	char				**tab;
	t_general			*head;

	i = 0;
	tab = ft_strsplit(line, ';');
	gen = gen_init();
	head = gen;
	while (tab[i])
	{
		gen->gen_line = ft_strdup(tab[i]);
		gen->nbr_pipes = ft_count_char(gen->gen_line, '|');
		if (!(gen->pipe_list = get_pp(gen->gen_line, gen->nbr_pipes, env, res)))
		{
			free_gen_list(gen);
			return (0);
		}
		if (tab[++i])
		{
			gen->next = gen_init();
			gen = gen->next;
		}
	}
	ft_str3del(tab);
	return (head);
}
