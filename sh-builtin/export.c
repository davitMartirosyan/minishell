/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 14:31:17 by sabazyan          #+#    #+#             */
/*   Updated: 2022/11/18 12:30:33 by sabazyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_header.h"

int	export_err(char *str)
{
	int		i;
	char	**key_val;

	i = 0;
	if (check_plus_equal(str))
	{
		key_val = ft_split(str, '+');
		if (!key_val[0])
			return (1);
		str = ft_split(key_val[0], '=')[0];
	}
	while (str[i])
	{
		if (!ft_isalpha(str[0]) && str[0] != '_')
			return (1);
		while (str[++i])
		{
			if (!ft_isalpha(str[i]) && !ft_isdigit(str[i]) && str[i] != '_')
				return (1);
		}
	}
	return (0);
}

int	check_key(char *str, t_table *tab)
{
	t_env	*temp;

	temp = tab->env;
	while (temp)
	{
		if (!ft_strcmp(temp->key, str))
			return (1);
		temp = temp->next;
	}
	return (0);
}

t_table	*create_tab(char **env)
{
	t_table	*tab;

	tab = malloc(sizeof(t_table));
	if (!tab || !env)
		return (NULL);
	//tab->status = 0;
	tab->env = env_tokenizing(env);
	return (tab);
}

int	check_plus_equal(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '+' && str[i + 1] && str[i + 1] == '=')
			return (-i);
		if (str[i] == '=')
			return (i);
	}
	return (0);
}

void	loop_plus_value(char *str, t_env *temp, int index, int len)
{
	while (temp)
	{
		if (!ft_strcmp(temp->key, ft_substr(str, 0, index)))
		{
			temp->val = ft_strjoin(ft_strtrim(temp->val, "\""),
					ft_strtrim(ft_substr(str, index + 2, len), "\""));
		}
		temp = temp->next;
	}
}
