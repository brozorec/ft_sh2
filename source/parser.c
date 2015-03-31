/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/22 16:52:07 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/31 20:06:49 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh1.h"
#include "ft_sh1_prototypes.h"

int
	check_if_home(char **cmd, char **env, t_res *res)
{
	char				*copy;

	if ((*cmd)[0] == '~')
	{
		if ((*cmd)[1] != 0 && (*cmd)[1] != '/')
		{
			err_msg("Unknown user: ");
			err_msg(&(*cmd)[1]);
			err_msg(".\n");
			return (-1);
		}
		copy = ft_strdup(*cmd);
		ft_strdel(cmd);
		*cmd = take_env_var("HOME=", ft_strchr(copy, '~') + 1, env);
		if (*cmd == 0)
			*cmd = ft_strjoin(res->home, ft_strchr(copy, '~') + 1);
		free(copy);
	}
	return (1);
}

int
	unmatched_quot(char *line)
{
	int					i;
	int					count;

	i = 0;
	count = 0;
	while (line[i])
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

char
	**get_cmd(char *line, char **env, t_res *res)
{
	int					i;
	int					len;
	char				**cmd;

	i = 0;
	if (unmatched_quot(line) == -1 || (len = count_args(line)) == 0)
	{
		// ft_putstr_fd("Invalid null command.\n", 2); // @> > toto
		return (0);
	}
	cmd = (char **)malloc(sizeof(char *) * (len + 1));
	cmd[len] = 0;
	cmd = split_line(line, cmd);
	// if (cmd[0] == 0)
	// {
	// 	ft_putstr_fd("Invalid null command.\n", 2);
	// 	return (0);
	// }
	while (cmd[i])
	{
		ft_trim_quot_marks(&cmd[i]);
		if (check_if_home(&(cmd)[i], env, res) == 0)
			return (0);
		++i;
	}
	return (cmd);
}
