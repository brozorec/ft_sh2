/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh1.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/20 15:51:32 by bbarakov          #+#    #+#             */
/*   Updated: 2015/03/29 18:11:30 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SH1_H
# define FT_SH1_H

# include <dirent.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include "get_next_line.h"
# include <stdio.h> // not authorized
# include <errno.h> // not

# define STDIN 0
# define STDOUT 1
# define READ_END 0
# define WRITE_END 1

typedef struct			s_cd
{
	int					opt_l;
	int					opt_p;
	char				*name;
	char				*path;
	char				*input;
	char				*saved_path;
	char				new_dir[4096];
	char				old_dir[4096];
}						t_cd;

typedef struct			s_reserved
{
	char				**paths;
	char				*home;
}						t_res;

/*  flag_1 = > 	*/
/*  flag_2 = >> */
/*  flag_3 = < 	*/
/*  flag_4 = << */

typedef struct			s_pipe
{
	int					num;
	char				*pipe_line;
	char				**cmd_tab;
	char				*file_for_out;
	char				*file_for_in;
	int					no_name;
	int					flag_1;
	int					flag_2;
	int					flag_3;
	int					flag_4;
	struct s_pipe		*next;
}						t_pipe;

typedef struct			s_general
{
	char				*gen_line;
	int					nbr_pipes;
	struct s_pipe		*pipe_list;
	struct s_general	*next;
}						t_general;

#endif
