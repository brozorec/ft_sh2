/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/28 13:20:48 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/31 19:57:36 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh1.h"
#include "ft_sh1_prototypes.h"

int
	check_flags(t_pipe *pipe)
{
	int					i;

	i = 0;
	if (pipe->flag_1 || pipe->flag_2 || pipe->flag_3 || pipe->flag_4)
		i = 1;
	return (i);
}

char
	*get_redir_file(char *line)
{
	char				*file;
	char				*str;
	int					len;
	int					i;

	file = 0;
	i = 0;
	str = line;
	while (line[i] == '>' || line[i] == '<')
		line[i++] = ' ';
	while (line[i] == ' ' || line[i] == '\t')
		++i;
	if (line[i] == '>' || line[i] == '<')
		return (0);
	len = ft_len_to_char(&line[i], ' ', '\t');
	file = ft_strnew(len);
	file = ft_strncpy(file, &line[i], len);
	if (ft_strlen(file) == 0)
	{
		ft_putstr_fd("Missing name for redirect.\n", 2);
		return (0);
	}
	str = ft_memset(&line[i], ' ', len);
	return (file);
}

t_pipe
	*get_redirections_input(t_pipe *pipe)
{
	char				*str;
	char				*save;

	save = pipe->pipe_line;
	while (save)
	{
		if ((str = ft_strstr(save, "<<")))
		{
			save = (str + 2);
			pipe->file_for_in = get_redir_file(str);
			++(pipe->flag_4);
		}
		else if ((str = ft_strstr(save, "<")))
		{
			save = (str + 1);
			pipe->file_for_in = get_redir_file(str);
			++(pipe->flag_3);
		}
		else
			save = str;
	}
	if (pipe->flag_4 > 1 || pipe->flag_3 > 1 || (pipe->flag_4 == 1 && pipe->flag_3 == 1))
		return (0);
	return (pipe);
}

t_pipe
	*get_redirections_output(t_pipe *pipe)
{
	char				*str;
	char				*save;

	save = pipe->pipe_line;
	while (save)
	{
		if ((str = ft_strstr(save, ">>")))
		{
			save = (str + 2);
			pipe->file_for_out = get_redir_file(str);
			++(pipe->flag_2);
		}
		else if ((str = ft_strstr(save, ">")))
		{
			save = (str + 1);
			pipe->file_for_out = get_redir_file(str);
			++(pipe->flag_1);
		}
		else
			save = str;
	}
	if (pipe->flag_2 > 1 || pipe->flag_1 > 1 || (pipe->flag_2 == 1 && pipe->flag_1 == 1))
		return (0);
	return (pipe);
}

t_pipe
	*examine_pipe_line(t_pipe *pipe, int nbr_pipes, char **env, t_res *res)
{
	if ((pipe = get_redirections_output(pipe)) == 0)
	{
		ft_putstr_fd("Ambiguous output redirect.\n", 2);
		return (0);
	}
	if (pipe->num != nbr_pipes && (pipe->flag_1 || pipe->flag_2))
	{
		ft_putstr_fd("Ambiguous output redirect.\n", 2);
		return (0);
	}
	if ((pipe = get_redirections_input(pipe)) == 0)
	{
		ft_putstr_fd("Ambiguous input redirect.\n", 2);
		return (0);
	}
	if (pipe->num > 0 && (pipe->flag_3 || pipe->flag_4))
	{
		ft_putstr_fd("Ambiguous input redirect.\n", 2);
		return (0);
	}
	if (!pipe->file_for_out && !pipe->file_for_in && check_flags(pipe))
		return (0);
	if ((pipe->cmd_tab = get_cmd(pipe->pipe_line, env, res)) == 0)
		return (0);
	return (pipe);
}

t_pipe
	*get_pipe_lists(char *gen_line, int nbr_pipes, char **env, t_res *res)
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
		if ((pipe_list = examine_pipe_line(pipe_list, nbr_pipes, env, res)) == 0)
			return (0);
		if (tab[++i])
		{
			pipe_list->next = pipe_init();
			pipe_list = pipe_list->next;
		}
	}
	if (nbr_pipes != i - 1)
	{
		ft_putstr_fd("Invalid null command.\n", 2);
		return (0);
	}
	pipe_list = head;
	return (pipe_list);
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
		if (!(gen->pipe_list = get_pipe_lists(gen->gen_line, gen->nbr_pipes, env, res)))
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
	gen = head;
	return (gen);
}
