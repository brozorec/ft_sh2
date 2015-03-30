/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/28 13:20:48 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/29 13:34:37 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh1.h"
#include "ft_sh1_prototypes.h"

int
	check_flags(t_pipe *pipe_list)
{
	int					i;

	i = 0;
	if (pipe_list->flag_1 || pipe_list->flag_2 || pipe_list->flag_3 || pipe_list->flag_4)
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
	{
		line[i] = ' ';
		++i;
	}
	while (line[i] == ' ' || line[i] == '\t')
		++i;
	len = ft_len_to_char(&line[i], ' ', '\t');
	file = ft_strnew(len);
	file = ft_strncpy(file, &line[i], len);
	str = ft_memset(&line[i], ' ', len);
	return (file);
}

t_pipe
	*get_redirections_input(t_pipe *pipe_list)
{
	char				*str;
	char				*save;

	save = pipe_list->pipe_line;
	while (save)
	{
		if ((str = ft_strstr(save, "<<")))
		{
			save = (str + 2);
			pipe_list->file_for_in = get_redir_file(str);
			++(pipe_list->flag_4);
		}
		else if ((str = ft_strstr(save, "<")))
		{
			save = (str + 1);
			pipe_list->file_for_in = get_redir_file(str);
			++(pipe_list->flag_3);
		}
		else
			save = str;
	}
	if (pipe_list->flag_4 > 1 || pipe_list->flag_3 > 1 || (pipe_list->flag_4 == 1 && pipe_list->flag_3 == 1))
		return (0);
	return (pipe_list);
}

t_pipe
	*get_redirections_output(t_pipe *pipe_list)
{
	char				*str;
	char				*save;

	save = pipe_list->pipe_line;
	while (save)
	{
		if ((str = ft_strstr(save, ">>")))
		{
			save = (str + 2);
			pipe_list->file_for_out = get_redir_file(str);
			++(pipe_list->flag_2);
		}
		else if ((str = ft_strstr(save, ">")))
		{
			save = (str + 1);
			pipe_list->file_for_out = get_redir_file(str);
			++(pipe_list->flag_1);
		}
		else
			save = str;
	}
	if (pipe_list->flag_2 > 1 || pipe_list->flag_1 > 1 || (pipe_list->flag_2 == 1 && pipe_list->flag_1 == 1))
		return (0);
	return (pipe_list);
}

t_pipe
	*examine_pipe_line(t_pipe *pipe_list, int nbr_pipes)
{
	// printf("%s\n", pipe_list->pipe_line);
	if ((pipe_list = get_redirections_output(pipe_list)) == 0)
	{
		ft_putstr_fd("Ambiguous output redirect.\n", 2);
		return (0);
	}
	if (pipe_list->num != nbr_pipes && (pipe_list->flag_1 || pipe_list->flag_2))
	{
		ft_putstr_fd("Ambiguous output redirect.\n", 2);
		return (0);
	}
	if ((pipe_list = get_redirections_input(pipe_list)) == 0)
	{
		ft_putstr_fd("Ambiguous input redirect.\n", 2);
		return (0);
	}
	if (pipe_list->num > 0 && (pipe_list->flag_3 || pipe_list->flag_4))
	{
		ft_putstr_fd("Ambiguous input redirect.\n", 2);
		return (0);
	}
	if (pipe_list->file_for_out == 0 && pipe_list->file_for_in == 0 && check_flags(pipe_list))
	{
		ft_putstr_fd("Missing name for redirect.\n", 2);
		return (0);
	}
	if ((pipe_list->cmd_tab = get_cmd(pipe_list->pipe_line)) == 0)
		return (0);
	return (pipe_list);
}

t_pipe
	*get_pipe_lists(char *gen_line, int nbr_pipes)
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
		if ((pipe_list = examine_pipe_line(pipe_list, nbr_pipes)) == 0)
			return (0);
		// printf("%s\n", pipe_list->pipe_line);
		if (tab[++i])
		{
			pipe_list->next = pipe_init();
			pipe_list = pipe_list->next;
		}
	}
	pipe_list = head;
	return (pipe_list);
}

t_general
	*get_gen_lists(char *line, t_general *gen_list)
{
	int					i;
	char				**tab;
	t_general			*head;

	i = 0;
	tab = ft_strsplit(line, ';');
	gen_list = gen_init();
	head = gen_list;
	while (tab[i])
	{
		gen_list->gen_line = ft_strdup(tab[i]);
		gen_list->nbr_pipes = ft_count_char(gen_list->gen_line, '|');
		if ((gen_list->pipe_list = get_pipe_lists(gen_list->gen_line, gen_list->nbr_pipes)) == 0)
			return (0);
		// printf("%s\n", gen_list->pipe_list->pipe_line);
		if (tab[++i])
		{
			gen_list->next = gen_init();
			gen_list = gen_list->next;
		}
	}
	gen_list = head;
	return (gen_list);
}
