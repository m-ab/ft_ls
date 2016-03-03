/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhier <mbouhier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/22 05:25:42 by maboukra          #+#    #+#             */
/*   Updated: 2016/02/26 15:13:15 by mbouhier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	init_var(t_env *e)
{
	e->nb_path = 0;
	e->nb_files = 0;
	e->uppercase_r = '\0';
	e->lowercase_r = '\0';
	e->uppercase_g = '\0';
	e->lowercase_g = '\0';
	e->a = '\0';
	e->t = '\0';
	e->l = '\0';
	e->minor_max = 0;
	e->major_max = 0;
	if (!(e->tmp = ft_memalloc(ft_strlen(CHECK_FLAGS) + 1)))
		error_param(2, 1, NULL);
}

void	init_var2(t_env *e)
{
	e->lowercase_r = ft_strchr(e->tmp, 'r') > 0 ? 1 : 0;
	e->uppercase_r = ft_strchr(e->tmp, 'R') > 0 ? 1 : 0;
	e->lowercase_g = ft_strchr(e->tmp, 'g') > 0 ? 1 : 0;
	e->uppercase_g = ft_strchr(e->tmp, 'G') > 0 ? 1 : 0;
	e->a = ft_strchr(e->tmp, 'a') > 0 ? 1 : 0;
	e->t = ft_strchr(e->tmp, 't') > 0 ? 1 : 0;
	e->l = ft_strchr(e->tmp, 'l') > 0 ? 1 : 0;
	if (e->lowercase_g)
		e->l = 1;
	free(e->tmp);
}

void	init_path(t_env *e)
{
	if (!(e->path = (char**)malloc(sizeof(char*) * 2)))
		error_param(2, 1, NULL);
	if (!(e->path[0] = ft_memalloc(2)))
		error_param(2, 1, NULL);
	e->path[0][0] = '.';
	e->path[1] = NULL;
	e->nb_path = 1;
}

void	sort_path(char **str)
{
	int 	i;
	char	*temp;

	i = -1;
	while (str[++i] && str[i + 1])
	{
		if (ft_strcmp(str[i], str[i + 1]) <= 0)
			continue ;
		else
		{
			temp = str[i];
			str[i] = str[i + 1];
			str[i + 1] = temp;
			i = -1;
		}
	}
}

int		main(int ac, char **av)
{
	t_env	e;

	init_var(&e);
	if (ac == 1)
		init_path(&e);
	else
		parser(ac, av, &e, 1);
	if (e.nb_path == 0 && e.nb_files == 0)
		return (0);
	 if (e.nb_path > 0)
		 sort_path(e.path);
	 if (e.nb_files > 0)
		 sort_path(e.files);
	get_data(&e);
	return (0);
}
