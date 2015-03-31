/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/20 15:50:48 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/31 19:50:15 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh1.h"
#include "ft_sh1_prototypes.h"

t_res		*get_reserve(char ***env)
{
	t_res				*res;

	res = 0;
	res = (t_res *)malloc(sizeof(t_res));
	res->paths = get_reserved_paths();
	res->home = take_env_var("HOME=", 0, *env);
	if (get_len(*env, 0) == 0)
		*env = res->paths;
	return (res);
}

void		exit_eof(char ***env, t_res **res)
{
	free_env_cmd_res(0, env, res);
	ft_putstr("exit\n");
	exit(0);
}

void		free_gen_list(t_general *gen_list)
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

int			proceed(char ***env, t_res **res)
{
	char				*line;
	t_general			*gen_list;

	gen_list = 0;
	if (get_next_line(0, &line) == 0)
		exit_eof(env, res);
	if (ft_strlen(line) == 0)
	{
		free(line);
		return (1);
	}
	if ((gen_list = get_gen_lists(line, gen_list, *env, *res)) == 0)
	{
		// ft_putstr_fd("main\n", 2);
		free(line);
		return (1);
	}
	execute_gen_lists(gen_list, env, res);
	free(line);
	free_gen_list(gen_list);
	return (0);
}

int			main(void)
{
	extern char			**environ;
	char				**env;
	t_res				*res;

	signals();
	env = set_my_env(environ, 0, 0, 0);
	res = get_reserve(&env);
	while (1)
	{
		ft_putstr("@>");
		proceed(&env, &res);
	}
	return (0);
}
