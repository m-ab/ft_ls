/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhier <mbouhier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/25 14:08:41 by gmarguer          #+#    #+#             */
/*   Updated: 2016/02/26 14:00:06 by mbouhier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_solo_file(t_info *list, t_env *e)
{
	if (e->l && !S_ISLNK(list->file_stat.st_mode))
		print_file_l(e, list, NULL, list->name);
	else if (S_ISLNK(list->file_stat.st_mode) && e->l)
		print_file_l(e, list, NULL, "./");
	else 
	{
		ft_putstr(list->name);
		ft_putstr(" ");
	}
}

void	print_param_files(t_env *e)
{
	int		i;
	t_info	*list;

	i = -1;
	if (!(list = (t_info*)malloc(sizeof(t_info))))
		error_param(2, 1, NULL);
	while (e->files[++i])
	{

		lstat(e->files[i], &list->file_stat);
		list->next = NULL;
		list->statut = 0;
		list->name = e->files[i];
		size_display(e, list);
		print_solo_file(list, e);
		if (e->l == 0 && i >= e->nb_files - 1)
			ft_putchar('\n');
	}
}

void	get_data(t_env *e)
{
	int		i;

	i = -1;
	if (e->nb_files > 0)
	{
		print_param_files(e);
		if (e->nb_path > 0)
			ft_putchar('\n');
	}
	while (e->path[++i])
	{
		if (e->nb_path > 1 || e->nb_files > 0)
		{
			ft_putstr(e->path[i]);
			ft_putstr(":\n");
		}
		add_and_sort(e->path[i], e, i);
		if (e->nb_path > 1 && i < e->nb_path - 1)
			ft_putchar('\n');
	}
}
