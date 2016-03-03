/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboukra <maboukra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/22 05:26:16 by maboukra          #+#    #+#             */
/*   Updated: 2016/02/23 22:41:25 by maboukra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	sorting_flags(t_env *e)
{
	int		i;
	int		y;
	int		k;
	char	*tmp;

	i = -1;
	k = -1;
	if (!(tmp = ft_memalloc(ft_strlen(e->tmp) + 1)))
		error_param(2, 1, NULL);
	ft_strcpy(tmp, e->tmp);
	while (CHECK_FLAGS[++i])
	{
		y = -1;
		while (tmp[++y])
			if (tmp[y] == CHECK_FLAGS[i])
				e->tmp[++k] = tmp[y];
	}
	free(tmp);
}

void	check_opt2(char *str, t_env *e)
{
	int		i;
	int		y;

	i = 0;
	while (str[++i])
	{
		if (!ft_strchr(CHECK_FLAGS, str[i]))
		{
			ft_putstr_fd("ft_ls: illegal option -- ", 2);
			ft_putchar_fd(str[i], 2);
			ft_putchar_fd('\n', 2);
			error_param(1, 1, NULL);
		}
	}
	if (i == 1)
		error_param(1, 1, NULL);
	i = -1;
	while (CHECK_FLAGS[++i])
	{
		y = -1;
		while (str[++y])
			if (CHECK_FLAGS[i] == str[y] && !ft_strchr(e->tmp, str[y]))
				e->tmp[ft_strlen(e->tmp)] = str[y];
	}
	sorting_flags(e);
}

int	check_opt1(char *str, t_env *e)
{
	if (str[0] && str[1] && !str[2] && str[0] == '-' && str[1] == '-')
		return (1);
	check_opt2(str, e);
	return (0);
}

int		last_link(char *file)
{
	char *str;
	struct stat buf2;

	str = ft_strnew(255);
	readlink(file, str, 255);
	if (lstat(str, &buf2) == -1)
		return (1);
	if (S_ISLNK(buf2.st_mode))
		return (last_link(str));
	else if (S_ISDIR(buf2.st_mode))
		return (0);
	return (1);
}

int		check_path(char *str, int type, t_env *e)
{
	struct stat buf;
	int			i;

	i = 0;
	while (str[++i])
		if (ft_isprint(str[i]) == 0)
		{
			if (type == 0)
				error_param(3, 3, str);
			return (0);
		}
	if (lstat(str, &buf) == -1)
	{
		if (type == 0)
			error_param(3, 3, str);
		return (0);
	}
	if (S_ISDIR(buf.st_mode) || (!e->l && S_ISLNK(buf.st_mode) && !last_link(str) ))
		return (1);
	return (2);
}

void		parser(int ac, char **av, t_env *e, int i)
{
	int j;
	int k;
	int test;

	k = 0;
	test = 0;
	if (ac > 1)
	{
		while (av[i] && av[i][0] == '-' && !test)
			test = check_opt1(av[i++], e);
		init_var2(e);
		test = 0;
		j = i;
		while (av[i])
		{
			test = check_path(av[i++], 0, e);
			e->nb_path += test == 1 ? 1 : 0;
			e->nb_files += test == 2 ? 1 : 0;
			k++;
		}
		if (e->nb_path > 0 || e->nb_files > 0)
			fill_path(e, j, av);
		else if (k == 0)
			init_path(e);
	}
}

int		fill_path(t_env *e, int j, char **av)
{
	int		i;
	int 	k;
	int 	test;

	i = 0;
	k = 0;
	test = 0;
	if (!(e->path = (char**)malloc(sizeof(char*) * (e->nb_path + 1))))
		error_param(2, 1, NULL);
	if (!(e->files = (char**)malloc(sizeof(char*) * (e->nb_files + 1))))
		error_param(2, 1, NULL);
	e->path[e->nb_path] = NULL;
	e->files[e->nb_files] = NULL;
	while (av[j]) 
	{	
		test = check_path(av[j], 1, e);
		if (test == 1)
		{
			if (!(e->path[i++] = ft_strdup(av[j])))
				error_param(2, 1, NULL);
		}
		else if (test == 2)
			if (!(e->files[k++] = ft_strdup(av[j])))
				error_param(2, 1, NULL);
		j++;
	}
	return (1);
}
