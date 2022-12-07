/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 20:34:37 by codespace         #+#    #+#             */
/*   Updated: 2022/12/07 18:34:01 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_header.h"

static int	cmd_check(t_cmds *cmd, char **paths);
static void	execute(t_cmdline **cmd, t_table **table, char **envp);
static void	combined_execution(int pip, t_cmdline **cmd, t_table **table, char **envp);

void	execution(t_cmdline **commands, t_table **table, char **envp)
{
	int pip;
    
	pip = pipes(&((*table)->token));
	if (pip == 0)
		execute(commands, table, envp);
	else if (pip > 0)
		combined_execution(pip, commands, table, envp);
	else
		return ;
}

static void execute(t_cmdline **cmd, t_table **table, char **envp)
{
    t_vars  v;
    
	if(!cmd && !*cmd && !table && !*table && !envp && !*envp)
		return ;
    v.dupcopy = dup(0);
    v.dupcopy2 = dup(1);
    v.built = find_in((*cmd)->cmds->arg_pack[0], (*table)->reserved);
    v.binar = cmd_check((*cmd)->cmds, (*table)->paths);
    dup2((*cmd)->cmds->i_stream, 0);
    dup2((*cmd)->cmds->o_stream, 1);
    if(v.built != -1)
        (*table)->builtin[v.built]((*cmd)->cmds, *table);
    else if(v.binar != -1)
    {
        (*cmd)->pid = fork(); 
        if((*cmd)->pid == 0)
        {
            execve((*cmd)->cmds->path, (*cmd)->cmds->arg_pack, 0);
            exit(1);
        }
        else
            waitpid(-1, 0, 0);
    }
    else
        printf("%s%s%s", SHELLERR, (*cmd)->cmds->arg_pack[0], COMMANDERR);
    dup2(v.dupcopy, 0);
    dup2(v.dupcopy2, 1);
    return ;
}

static void combined_execution(int pip, t_cmdline **cmd, t_table **table, char **envp)
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
        (*cmd)->pid = fork();
        if((*cmd)->pid == 0)
        {
            v.built = find_in((*cmd)->cmds->arg_pack[0], (*table)->reserved);
            v.binar = cmd_check((*cmd)->cmds, (*table)->paths);        
            if(i == 0)
            {
                dup2(pips[i][0], (*cmd)->cmds->o_stream);
                printf("It is a first pipe\n");
            }
            else if(i > 0 && i < pip)
            {
                dup2(pips[i-1][1], (*cmd)->cmds->i_stream);
                printf("In the middle\n");
            }
            else
            {
                dup2(pips[i][1], (*cmd)->cmds->i_stream);
                printf("Last pipe\n");
            }
            if(v.built != -1)
                (*table)->builtin[v.built]((*cmd)->cmds, *table);
            else if(v.binar != -1)
                execve((*cmd)->cmds->path, (*cmd)->cmds->arg_pack, 0);
            else
                printf("%s%s%s", SHELLERR, (*cmd)->cmds->arg_pack[0], COMMANDERR);
            exit(1);
        }
        else
            waitpid(-1, 0, 0);
        i++;
        (*cmd)->cmds = (*cmd)->cmds->next;
    }
}

static int	cmd_check(t_cmds *cmd, char **paths)
{
	int		i;
	char	*path;

	i = 0;
	path = NULL;
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
	if (access(cmd->arg_pack[0], F_OK & X_OK) == 0)
	{
		cmd->path = ft_strdup(cmd->arg_pack[0]);
		cmd->exit_status = EXIT_SUCCESS;
		return (0);
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