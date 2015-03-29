/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/20 15:50:48 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/29 18:43:54 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh1.h"
#include "ft_sh1_prototypes.h"

t_res		*get_reserve(char **env)
{
	t_res				*res;

	res = 0;
	res = (t_res *)malloc(sizeof(t_res));
	res->paths = get_reserved_paths();
	res->home = take_env_var("HOME=", 0, env);
	return (res);
}

void		exit_eof(char ***cmd, char ***env, t_res **res)
{
	free_env_cmd_res(cmd, env, res);
	ft_putstr("exit\n");
	exit(0);
}

// void		print_tab(char **tab)
// {
// 	int					i;

// 	i = 0;
// 	while (tab[i])
// 	{
// 		printf("%s\n", tab[i]);
// 		++i;
// 	}
// 	printf("%s\n", "\n");
// }

// void		print(t_general *g_list)
// {
// 	while (g_list)
// 	{
// 		while (g_list->pipe_list)
// 		{
// 			print_tab(g_list->pipe_list->cmd_tab);
// 			g_list->pipe_list = g_list->pipe_list->next;
// 		}
// 		g_list = g_list->next;
// 	}
// }

int			execute_gen_lists(t_general *gen_list, char ***env, t_res **res)
{
	while (gen_list)
	{
		execute_pipe_lists(gen_list->pipe_list, gen_list->nbr_pipes, *env, *res);
		gen_list = gen_list->next;
	}
	return (0);
}

int			proceed(char ***env, t_res **res)
{
	char				*line;
	t_general			*gen_list;

	gen_list = 0;
	if (get_next_line(0, &line) == 0)
		exit(0);		// exit_eof(cmd, env, res);
	if (ft_strlen(line) == 0)
	{
		free(line);
		return (1);
	}
	if ((gen_list = get_gen_lists(line, gen_list)) == 0)
	{
		printf("%s\n", "main");
		free(line);
		return (1);
	}
	// print(gen_list);
	execute_gen_lists(gen_list, env, res);
	return (0);
}

int			main(void)
{
	extern char			**environ;
	char				**env;
	t_res				*res;

	signals();
	res = get_reserve(environ);
	env = set_my_env(environ, 0, 0, 0);
	ft_putstr("@>");
	while (1)
	{
		proceed(&env, &res);
		ft_putstr("@>");
	}
	return (0);
}
