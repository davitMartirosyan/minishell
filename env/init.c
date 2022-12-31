/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   init.c											 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: dmartiro <dmartiro@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/09/27 03:09:29 by dmartiro		  #+#	#+#			 */
/*   Updated: 2022/12/18 15:00:30 by dmartiro		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell_header.h"

void	bash_setup(t_table **table, char **envp)
{
	(void)envp;
	*table = malloc(sizeof(t_table));
	if (!table)
		return ;
	(*table)->env = env_tokenizing(envp);
	(*table)->hdocs = 0;
	(*table)->get_heredoc_flag = 0;
	(*table)->get_pid = get_pid();
	(*table)->status = 0;
	reserved(table);
}

void	reserved(t_table **table)
{
	(*table)->builtin[0] = print_echo;
	(*table)->builtin[1] = print_pwd;
	(*table)->builtin[2] = print_cd;
	(*table)->builtin[3] = ft_unset;
	(*table)->builtin[4] = ft_export;
	(*table)->builtin[5] = ft_exit;
	(*table)->builtin[6] = print_env;
	(*table)->reserved[0] = "echo";
	(*table)->reserved[1] = "pwd";
	(*table)->reserved[2] = "cd";
	(*table)->reserved[3] = "unset";
	(*table)->reserved[4] = "export";
	(*table)->reserved[5] = "exit";
	(*table)->reserved[6] = "env";
	(*table)->reserved[7] = NULL;
}

int get_pid()
{
	pid_t pid;
	int status;
	
	pid =  fork();
	if (pid == 0)
		exit(0);
	else
		wait(&status);
	return ((int)pid-1);
}