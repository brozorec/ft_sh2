/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/28 13:20:48 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/28 19:25:01 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh1.h"
#include "ft_sh1_prototypes.h"

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
			// printf("%s\n", pipe_list->pipe_line);
			pipe_list->file_for_out = get_redir_file(str);
			// printf("%s\n", pipe_list->pipe_line);
			// printf("%s\n", pipe_list->file_for_out);
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
	*examine_pipe_line(t_pipe *pipe_list)
{
	// printf("%s\n", pipe_list->pipe_line);
	if ((pipe_list = get_redirections_output(pipe_list)) == 0)
	{
		ft_putstr_fd("Ambiguous output redirect.\n", 2);
		return (0);
	}
	// if ((pipe_list = get_redirections_input(pipe_list)) == 0)
	// {
	// 	ft_putstr_fd("Ambiguous input redirect.\n", 2);
	// 	return (0);
	// }
	// if (pipe_list->file_for_out == 0 && pipe_list->file_for_in == 0) //more conditions for the flags
	// {
	// 	ft_putstr_fd("Missing name for redirect.\n", 2);
	// 	return (0);
	// }
	return (pipe_list);
}

t_pipe
	*get_pipe_lists(char *gen_line)
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
		if ((pipe_list = examine_pipe_line(pipe_list)) == 0)
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

int 		count(char *str, int ch)
{
	int 				i;
	int 				count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (ch == str[i])
			++count;
		++i;
	}
	return (count);
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
			gen_list->nbr_pipes = count(gen_list->gen_line, '|');
		if ((gen_list->pipe_list = get_pipe_lists(gen_list->gen_line)) == 0)
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
