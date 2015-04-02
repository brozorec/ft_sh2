/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/22 16:52:07 by bbarakov          #+#    #+#             */
/*   Updated: 2015/04/02 14:55:31 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh2.h"
#include "ft_sh2_prototypes.h"

int
	check_if_home(char **cmd, char **env, t_res *res)
{
	char				*copy;

	if ((*cmd)[0] == '~')
	{
		copy = ft_strdup(*cmd);
		ft_strdel(cmd);
		*cmd = take_env_var("HOME=", ft_strchr(copy, '~') + 1, env);
		if (*cmd == 0)
			*cmd = ft_strjoin(res->home, ft_strchr(copy, '~') + 1);
		free(copy);
	}
	return (0);
}

int
	unmatched_quot(char *line)
{
	int					i;
	int					count;

	i = 0;
	count = 0;
	while (line && line[i])
	{
		if (line[i] == '"')
			++count;
		++i;
	}
	if (count % 2 != 0)
	{
		err_msg("Unmatched \".\n");
		return (-1);
	}
	return (0);
}

int
	count_args(char *line)
{
	int					i;
	int					count;

	i = 0;
	count = 0;
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			++i;
		if (line[i] == '"')
		{
			++i;
			i += ft_len_to_char(&line[i], '"', '"');
			++count;
		}
		else if (line[i] != '\0')
		{
			i += ft_len_to_char(&line[i], ' ', '\t');
			++count;
		}
		if (line[i] != '\0')
			++i;
	}
	return (count);
}

char
	**split_line(char *line, char **cmd)
{
	int					i;
	int					j;
	int					len;

	i = 0;
	j = 0;
	len = 0;
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			++i;
		if (line[i] == '"')
			len = ft_len_to_char(&line[++i], '"', '"');
		else if (line[i] != '\0')
			len = ft_len_to_char(&line[i], ' ', '\t');
		if (len != 0)
		{
			cmd[j++] = ft_strsub(line, i, len);
			i += len;
		}
		len = 0;
		if (line[i] != '\0')
			++i;
	}
	return (cmd);
}

t_pipe
	*get_cmd(t_pipe *pipe, char **env, t_res *res)
{
	int					i;
	int					len;

	i = 0;
	if (unmatched_quot(pipe->pipe_line) == -1 ||
		unmatched_quot(pipe->file_for_in) == -1 ||
		unmatched_quot(pipe->file_for_out) == -1)
		return (0);
	if ((len = count_args(pipe->pipe_line)) == 0)
	{
		ft_putstr_fd("Invalid null command.\n", 2);
		return (0);
	}
	pipe->cmd_tab = (char **)malloc(sizeof(char *) * (len + 1));
	pipe->cmd_tab[len] = 0;
	pipe->cmd_tab = split_line(pipe->pipe_line, pipe->cmd_tab);
	ft_trim_quot_marks(&(pipe->file_for_out));
	ft_trim_quot_marks(&(pipe->file_for_in));
	while (pipe->cmd_tab[i])
	{
		ft_trim_quot_marks(&(pipe->cmd_tab[i]));
		check_if_home(&(pipe->cmd_tab)[i], env, res);
		++i;
	}
	return (pipe);
}
