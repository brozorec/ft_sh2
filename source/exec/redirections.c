/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/31 17:57:49 by bbarakov          #+#    #+#             */
/*   Updated: 2015/04/02 14:56:29 by bbarakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sh2.h"
#include "ft_sh2_prototypes.h"

void		redir_here_document(char *tag, int *fd_tab, int nbr_pipes)
{
	char		*buff;
	int			len;
	int			fd_new_tab[2];

	buff = 0;
	pipe(fd_new_tab);
	if (fork() == 0)
	{
		close_fd_tab(fd_tab, nbr_pipes);
		dup2(fd_new_tab[WRITE_END], STDOUT);
		close(fd_new_tab[READ_END]);
		while (get_next_line(0, &buff) != 0 && ft_strcmp(buff, tag))
		{
			len = ft_strlen(buff);
			buff[len] = '\n';
			write(STDOUT, buff, len + 1);
		}
		exit(0);
	}
	dup2(fd_new_tab[READ_END], STDIN);
	close(fd_new_tab[WRITE_END]);
	wait(0);
}

void		redir_flag_3(t_pipe *pipe_list)
{
	int			fd_file;

	if ((fd_file = open(pipe_list->file_for_in, O_RDWR)) == -1)
	{
		err_msg(pipe_list->file_for_in);
		err_msg(": No such file or directory.\n");
		exit(0);
	}
	dup2(fd_file, STDIN);
	close(fd_file);
}

void		redirections_files(t_pipe *pipe_list, int *fd_tab, int nbr_pipes)
{
	int			fd_file;

	if (pipe_list->flag_1)
	{
		fd_file = open(pipe_list->file_for_out,
			O_TRUNC | O_CREAT | O_RDWR, ACCESSPERMS);
		dup2(fd_file, STDOUT);
		close(fd_file);
	}
	else if (pipe_list->flag_2)
	{
		fd_file = open(pipe_list->file_for_out,
			O_CREAT | O_APPEND | O_RDWR, ACCESSPERMS);
		dup2(fd_file, STDOUT);
		close(fd_file);
	}
	if (pipe_list->flag_3)
		redir_flag_3(pipe_list);
	else if (pipe_list->flag_4)
		redir_here_document(pipe_list->file_for_in, fd_tab, nbr_pipes);
}

void		redirections_pipes(t_pipe *pipe_list, int *fd_tab, int j)
{
	if (pipe_list->flag_1 == 0 && pipe_list->flag_2 == 0)
	{
		if (pipe_list->next)
			dup2(fd_tab[j + 1], STDOUT);
	}
	if (pipe_list->flag_3 == 0 && pipe_list->flag_4 == 0)
	{
		if (pipe_list->num != 0)
			dup2(fd_tab[j - 2], STDIN);
	}
}
