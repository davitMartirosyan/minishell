/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   execution.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: dmartiro <dmartiro@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/11/26 20:34:37 by codespace		 #+#	#+#			 */
/*   Updated: 2022/12/19 10:37:09 by dmartiro		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell_header.h"

void	execution(t_cmdline **commands, t_table **table)
{
	int pip;

	if (commands && *commands)
	{
		pip = pipes(&((*table)->token));
		if (pip == 0)
			execute(*commands, *table);
		else if (pip > 0)
			piping_execute(pip, *commands, *table);
	}
}

void	execute(t_cmdline *cmd, t_table *table)
{
	t_vars v;

	if (!cmd->cmds->arg_pack)
		return;
	table->status = 0;
	table->dup0 = dup(0);
	table->dup1 = dup(1);
	v.built = find_in(cmd->cmds->arg_pack[0], table->reserved);
	v.binar = cmd_check(cmd->cmds, table);
	dup2(cmd->cmds->i_stream, 0);
	dup2(cmd->cmds->o_stream, 1);
	_execute(&v, cmd, table);
	dup2(table->dup0, 0);
	dup2(table->dup1, 1);
	close(table->dup0);
	close(table->dup1);
}

void	piping_execute(int pip, t_cmdline *cmd, t_table *table)
{
	t_vars v;
	t_cmds *cmds;
	int (*pip_ptr)[2];
	int i;
	int ccount;

	cmds = cmd->cmds;
	table->status = 0;
	pip_ptr = malloc(sizeof(*pip_ptr) * pip);
	if (!pip_ptr)
		return ;
	i = -1;
	while (++i < pip)
		pipe(pip_ptr[i]);
	ccount = _execute_pipes(cmds, &v, table, pip_ptr);
	i = -1;
	close_all_pipes(pip_ptr, pip);
	while (ccount--)
		wait(&table->status);
	free(pip_ptr);
}
// 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