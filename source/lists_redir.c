/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/01 14:38:58 by bbarakov          #+#    #+#             */
/*   Updated: 2015/04/01 19:14:20 by bbarakov         ###   ########.fr       */
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
	while (line[i] == '<' || line[i] == '>')
		line[i++] = ' ';
	while (line[i] == ' ' || line[i] == '\t')
		++i;
	len = ft_len_to_char(&line[i], ' ', '\t');
	file = ft_strnew(len);
	file = ft_strncpy(file, &line[i], len);
	if (!ft_strlen(file) || ft_strchr(file, '<') || ft_strchr(file, '>'))
	{
		free(file);
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
			if ((pipe->file_for_in = get_redir_file(str)) == 0)
				pipe->no_name = 1;
			++(pipe->flag_4);
		}
		else if ((str = ft_strstr(save, "<")))
		{
			save = (str + 1);
			if ((pipe->file_for_in = get_redir_file(str)) == 0)
				pipe->no_name = 1;
			++(pipe->flag_3);
		}
		else
			save = str;
	}
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
			if ((pipe->file_for_out = get_redir_file(str)) == 0)
				pipe->no_name = 1;
			++(pipe->flag_2);
		}
		else if ((str = ft_strstr(save, ">")))
		{
			save = (str + 1);
			if ((pipe->file_for_out = get_redir_file(str)) == 0)
				pipe->no_name = 1;
			++(pipe->flag_1);
		}
		else
			save = str;
	}
	return (pipe);
}
