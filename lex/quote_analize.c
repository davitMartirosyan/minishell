/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   quote_analize.c									:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: dmartiro <dmartiro@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/10/15 20:05:04 by root			  #+#	#+#			 */
/*   Updated: 2022/12/17 10:59:20 by dmartiro		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell_header.h"

void	openquotes(char *cmdline)
{
	int i;

	i = -1;
	while (cmdline[++i])
	{
		if (cmdline[i] == '\'')
			inside_quote(cmdline, &i, DBL);
		if (cmdline[i] == '\"')
			inside_quote(cmdline, &i, SGL);
	}
}

int check_quotes(char *cmdline, int *q_c)
{
	int i;
	int dbl;
	int sgl;

	dbl = 0;
	sgl = 0;
	i = -1;
	while (cmdline[++i])
	{
		if (cmdline[i] && cmdline[i] == '\"')
			dbl++;
		else if (cmdline[i] && cmdline[i] == '\'')
			sgl++;
	}
	q_c[0] = dbl;
	q_c[1] = sgl;
	if ((dbl % 2 != 0) || (sgl % 2 != 0))
		return (0);
	else 
		return (1);
}

void	inside_quote(char *cmdline, int *pos, int find)
{
	int i;

	i = *pos;
	if (find == SGL)
	{
		while (cmdline[++i])
		{
			if (cmdline[i] && cmdline[i] == '\"')
				break;
			if (cmdline[i] && cmdline[i] == '\'')
				cmdline[i] = 7;
		}
	}
	else if (find == DBL)
	{
		while (cmdline[++i])
		{
			if (cmdline[i] && cmdline[i] == '\'')
				break;
			if (cmdline[i] && cmdline[i] == '\"')
				cmdline[i] = 8;
			if (cmdline[i] && cmdline[i] == '$')
				cmdline[i] = 4;
		}
	}
	*pos = i;
}

int quote_syntax_analyzer(char *cmdline, int *q_c)
{
	if (check_quotes(cmdline, q_c))
	{
		token_replacment(cmdline, 7, '\'');
		token_replacment(cmdline, 8, '\"');
		return (1);
	}
	else
	{
		token_replacment(cmdline, 7, '\'');
		token_replacment(cmdline, 8, '\"');
		return (0);
	}
}

char	*exit_status_code(char *cmd, t_table *table, int *pos)
{
	char *status;
	char *left;
	char *newpoint;

	(void)table;
	(void)pos;
	status = ft_itoa(table->status);
	left = ft_substr(cmd, 0, *pos);
	newpoint = ft_strjoin(left, status);
	newpoint = ft_strjoin(newpoint, cmd+(*pos+2));
	free(cmd);
	free(status);
	return (newpoint);
}