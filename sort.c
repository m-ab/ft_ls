/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhier <mbouhier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/22 03:00:26 by maboukra          #+#    #+#             */
/*   Updated: 2016/02/26 13:59:02 by mbouhier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		ascii_sort(t_info **begin, t_info *to_add)
{
	t_info	*current_list;

	current_list = *begin;
	if ((ft_strcmp(to_add->name,
						current_list->name) < 0))
	{
		to_add->next = current_list;
		*begin = to_add;
		return ;
	}
	while (current_list->next)
	{
		if ((ft_strcmp(to_add->name,
						current_list->name) > 0) &&
			(ft_strcmp(to_add->name,
						current_list->next->name) < 0))
		{
			to_add->next = current_list->next;
			current_list->next = to_add;
			return ;
		}
		current_list = current_list->next;
	}
	current_list->next = to_add;
	to_add->next = NULL;
}

void		reverse_sort(t_info **begin, t_info *to_add)
{
	t_info	*current_list;

	current_list = *begin;
	if ((ft_strcmp(to_add->name,
						current_list->name) > 0))
	{
		to_add->next = current_list;
		*begin = to_add;
		return ;
	}
	while (current_list->next)
	{
		if ((ft_strcmp(to_add->name,
						current_list->name) < 0) &&
			(ft_strcmp(to_add->name,
						current_list->next->name) > 0))
		{
			to_add->next = current_list->next;
			current_list->next = to_add;
			return ;
		}
		current_list = current_list->next;
	}
	current_list->next = to_add;
	to_add->next = NULL;
}

long int		timecmp(struct stat time1, struct stat time2)
{
	long int r;

	r = time1.st_mtimespec.tv_sec - time2.st_mtimespec.tv_sec;
	if (!r)
		r = time1.st_mtimespec.tv_nsec - time2.st_mtimespec.tv_nsec;
	return (r);

}

void		time_sort(t_info **begin, t_info *to_add)
{
	t_info	*current_list;

	current_list = *begin;
	if (timecmp(to_add->file_stat, current_list->file_stat) > 0)
	{
		to_add->next = current_list;
		*begin = to_add;
		return ;
	}
	while (current_list->next)
	{
		if (timecmp(to_add->file_stat, current_list->file_stat) <= 0
			&& timecmp(to_add->file_stat, current_list->next->file_stat) > 0)
		{
			to_add->next = current_list->next;
			current_list->next = to_add;
			return ;
		}
		current_list = current_list->next;
	}
	current_list->next = to_add;
	to_add->next = NULL;
}

void		reverse_time_sort(t_info **begin, t_info *to_add)
{
	t_info	*current_list;

	current_list = *begin;
	if (timecmp(to_add->file_stat, current_list->file_stat) < 0)
	{
		to_add->next = current_list;
		*begin = to_add;
		return ;
	}
	while (current_list->next)
	{
		if (timecmp(to_add->file_stat, current_list->file_stat) >= 0
			&& timecmp(to_add->file_stat, current_list->next->file_stat) < 0)
		{
			to_add->next = current_list->next;
			current_list->next = to_add;
			return ;
		}
		current_list = current_list->next;
	}
	current_list->next = to_add;
	to_add->next = NULL;
}

void		sort(t_env *e, t_info *to_add, t_info **begin)
{
	if (e->lowercase_r && e->t)
	 	reverse_time_sort(begin, to_add);
	else if (e->t)
		time_sort(begin, to_add);
	else if (e->lowercase_r)
		reverse_sort(begin, to_add);
	else
		ascii_sort(begin, to_add);
}
