/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 20:19:57 by dmartiro          #+#    #+#             */
/*   Updated: 2022/12/04 23:16:01 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell_header.h"

static void reset_update_table(t_table **table, t_cmdline **tree);
static void print_tokens(t_tok **token);


int main(int argc, char *argv[], char *envp[])
{
    (void)argv;
    (void)envp;
    (void)argc;
    t_table     *table;
    t_cmdline   *tree;
    char        *cmdline;
    
    (void)tree;
	tree = NULL;
    create_shell(envp, &table);    
    while(1)
    {
        cmdline = readline("Minishell-$ ");
        add_history(cmdline);
        lexical_analyzer(cmdline, table);
        tree = parse_tree(table, envp);
        execution(&tree, &table, envp);
        reset_update_table(&table, &tree);
        print_tokens(&table->token);
    }
    return (0);
}