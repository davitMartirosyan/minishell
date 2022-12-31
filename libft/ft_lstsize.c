/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_lstsize.c									   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: dmartiro <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/04/03 22:06:52 by dmartiro		  #+#	#+#			 */
/*   Updated: 2022/04/03 22:09:04 by dmartiro		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
