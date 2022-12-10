/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 20:34:37 by codespace         #+#    #+#             */
/*   Updated: 2022/12/10 22:42:29 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_header.h"

static int	cmd_check(t_cmds *cmd, char **paths);
static void	execute(t_cmdline **cmd, t_table **table);
static void	combined_execution(int pip, t_cmdline **cmd, t_table **table);

void	execution(t_cmdline **commands, t_table **table)
{
	int pip;

    if(commands && *commands)
    {
        pip = pipes(&((*table)->token));
        if (pip == 0)
            execute(commands, table);
        else if (pip > 0)
            combined_execution(pip, commands, table);
        else
            return ;
    }
}

static void execute(t_cmdline **cmd, t_table **table)
{
    t_vars  v;
	if(!cmd && !*cmd && !table && !*table)
		return ;
	else if((*cmd)->cmds->arguments == NULL)	
		return ;
    v.dupcopy = dup(0);
    v.dupcopy2 = dup(1);
    v.built = find_in((*cmd)->cmds->arg_pack[0], (*table)->reserved);
    v.binar = cmd_check((*cmd)->cmds, (*table)->paths);
	dup2((*cmd)->cmds->i_stream, 0);
    dup2((*cmd)->cmds->o_stream, 1);
    if(v.built != -1)
        (*table)->builtin[v.built]((*cmd)->cmds, *table);
    else if(!v.binar)
    {
        (*cmd)->cmds->pid = fork(); 
        if((*cmd)->cmds->pid == 0)
        {
            execve((*cmd)->cmds->path, (*cmd)->cmds->arg_pack, 0);
            exit(1);
        }
        else
            waitpid(-1, 0, 0);
    }
    else if(v.binar == -2)
        printf("%s%s%s", SHELLERR, (*cmd)->cmds->arg_pack[0], FILEERR);
    else
        printf("%s%s%s", SHELLERR, (*cmd)->cmds->arg_pack[0], COMMANDERR);
    dup2(v.dupcopy, 0);
    dup2(v.dupcopy2, 1);
    return ;
}

static void combined_execution(int pip, t_cmdline **cmd, t_table **table)
{
    t_vars v;
    int i;
    int (*pips)[2];

    pips = malloc(sizeof(*pips) * pip);
    if(!pips)
        return ;
    i = -1;
    while(++i < pip)
        pipe(pips[i]);
    i = 0;
    while((*cmd)->cmds != NULL)
    {
        (*cmd)->cmds->pid = fork();
        if((*cmd)->cmds->pid == 0)
        {
            v.built = find_in((*cmd)->cmds->arg_pack[0], (*table)->reserved);
            v.binar = cmd_check((*cmd)->cmds, (*table)->paths);
            if(i == 0)
            {
                dup2(pips[i][1], (*cmd)->cmds->o_stream);
                close(pips[i][0]);
            }
            else if(i > 0 && i < pip)
            {
                dup2(pips[i-1][0], (*cmd)->cmds->i_stream);
                dup2(pips[i][1], (*cmd)->cmds->o_stream);
                close(pips[i-1][1]);
                close(pips[i][0]);
            }
            else
            {
                dup2(pips[i-1][0], (*cmd)->cmds->i_stream);
                close(pips[i-1][1]);
            }
            if(v.built != -1)
                (*table)->builtin[v.built]((*cmd)->cmds, *table);
            else if(v.binar != -1)  
                execve((*cmd)->cmds->path, (*cmd)->cmds->arg_pack, 0);
            exit(1);
        }
        else
        {
            i++;
            (*cmd)->cmds = (*cmd)->cmds->next;
        }
    }
    waitpid(-1, 0, 0);
}

static int	cmd_check(t_cmds *cmd, char **paths)
{
	int		i;
	char	*path;

	i = 0;
	path = NULL;
    if (access(cmd->arg_pack[0], F_OK & X_OK) == 0)
	{
		cmd->path = ft_strdup(cmd->arg_pack[0]);
		cmd->exit_status = EXIT_SUCCESS;
		return (0);
	}
    if(paths == NULL)
        return (-2);
	while (paths[i])
	{
		path = join_arguments(paths[i], '/', cmd->arg_pack[0]);
		if (access(path, F_OK & X_OK) == 0)
		{
			cmd->path = ft_strdup(path);
			cmd->exit_status = EXIT_SUCCESS;
			free(path);
			return (0);
		}
		free(path);
		i++;
	}
	return (-1);
}

int find_in(char *builtin, char **reserved)
{
    t_vars v;
    
    v.let = -1;
    v.var = 0;
    v.def = ft_strlen(builtin);
    while(reserved[++v.let]);
    while(reserved[v.var])
    {
        v.log = ft_strlen(reserved[v.var]);
        if(ft_strncmp(builtin, reserved[v.var], v.log) == 0 && \
            v.def == v.log)
            break;
        v.var++;
    }
    if(v.var == v.let && ft_strncmp(builtin, reserved[v.var-1], \
        ft_strlen(builtin)) != 0)
        return (-1);
    return (v.var);
}