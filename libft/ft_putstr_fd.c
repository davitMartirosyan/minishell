/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_putstr_fd.c									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: dmartiro <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2022/03/24 20:19:08 by dmartiro		  #+#	#+#			 */
/*   Updated: 2022/04/05 21:49:43 by dmartiro		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = 0;
	if (fd < 0 || !s)
		return ;
	while (s[i] != '\0')
		i++;
	write (fd, s, i);
}
