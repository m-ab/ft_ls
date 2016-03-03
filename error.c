/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhier <mbouhier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/22 05:26:33 by maboukra          #+#    #+#             */
/*   Updated: 2016/02/26 13:59:41 by mbouhier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		error_param(int type, int ex, char *str)
{
	if (type == 1)
	{
		ft_putstr_fd("\033[0musage: ./ft_ls \033[32m[-RatrlgG]\
\033[0m [file ...]\n", 2);
	}
	else if (type == 2)
		ft_putstr_fd("Malloc error.\n", 2);
	else if (type == 3)
	{
		ft_putstr_fd("\033[31mft_ls: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd("\033[0m", 2);
		perror(0);
	}
	if (ex == 1)
		exit(EXIT_SUCCESS);
	if (ex == 3)
		return ;
}