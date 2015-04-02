/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/21 19:22:17 by bbarakov          #+#    #+#             */
/*   Updated: 2015/04/01 13:23:35 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh2.h"
#include "ft_sh2_prototypes.h"

int			compare(char *cmd, char *env)
{
	int			i;
	char		*var_name_env;

	i = 0;
	while (env && env[i] && env[i] != '=')
		++i;
	var_name_env = ft_strsub(env, 0, i);
	if (ft_strcmp(cmd, var_name_env) == 0)
	{
		free(var_name_env);
		return (i);
	}
	free(var_name_env);
	return (0);
}

int			proceed_unset(char *var_name, char ***env)
{
	int			i;
	int			cmp;

	i = 0;
	if (ft_strcmp(var_name, "*") == 0)
	{
		ft_str3del(*env);
		*env = 0;
		return (0);
	}
	while (*env && (*env)[i])
	{
		if ((cmp = compare(var_name, (*env)[i])) != 0)
		{
			*env = set_my_env(*env, var_name, cmp, -1);
			return (1);
		}
		++i;
	}
	return (1);
}

void		unsetenv_builtin(char **cmd, char ***env)
{
	int			i;

	i = 1;
	if (cmd[i] == 0)
	{
		err_msg("unsetenv: Too few arguments.\n");
		return ;
	}
	while (cmd[i])
	{
		if (proceed_unset(cmd[i], env) == 0)
			return ;
		++i;
	}
}
