/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh1_prototypes.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/04 18:15:24 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/30 14:38:40 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SH1_PROTOTYPES_H
# define FT_SH1_PROTOTYPES_H

void	signals(void);
char	*lookup_paths(char *var, char *name, char **env);
char	**get_paths(char *var, char **env);
t_res	*get_reserve(char **env);
char	**get_reserved_paths(void);
char	*dir_content(char *path, char *name);

void	execute_command(char **cmd, char **env, t_res *res);
void	execute_pipe_lists(t_pipe *pipe_list, int nbr_pipes, char ***env, t_res **res);
void	close_fd_tab(int *fd_tab, int nbr_pipes);
int		*create_fd_tab(int nbr_pipes);
void	wait_children(int nbr_pipes);

int		is_builtin(char **cmd);
int		execute_builtin(char **cmd, char ***environ, t_res **res);
char	**set_my_env(char **environ, char *str, int cmp, int flag);
void	setenv_builtin(char **cmd, char ***env, t_res **res);
void	unsetenv_builtin(char **cmd, char ***env);
void	env_builtin(char **cmd, char **env);
void	print_env(char **env);
void	exit_builtin(char **cmd, char ***env, t_res *res);
void	free_env_cmd_res(char ***cmd, char ***env, t_res **res);
void	cd_builtin(char **cmd, char ***env, t_res **res);
int		examine_path(t_cd **lst);
char	*construct_path(t_cd *lst);
void	pwd_builtin(char **env);
void	lst_init_or_free(t_cd **lst);
void	change_or_add_env_var(char *var, char *value, char ***env);
char	*take_env_var(char *var, char *addr, char **env);
int		get_options(char *cmd, t_cd **lst);
int		take_oldpwd(char **cmd, t_cd **lst, char **env);
char	*second_try(char *name, char **env);
int		check_too_many_args(t_cd *lst, char **cmd);
int		get_len(char **ptr, int flag);
int		compare(char *cmd, char *env);

char	**get_cmd(char *line);
char	*ft_trim_quot_marks(char *str);
int		ft_len_to_char(char *str, char ch1, char ch2);
int		ft_count_char(char *str, int ch);

void	err_msg(char *err);
void	cd_errors(t_cd **lst);
void	cd_options_err(char a);
void	exit_err(int i);


t_general	*get_gen_lists(char *line, t_general *gen_list);
t_general	*gen_init(void);
t_pipe		*pipe_init(void);

#endif
