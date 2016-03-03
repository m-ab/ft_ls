/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhier <mbouhier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/25 14:13:56 by gmarguer          #+#    #+#             */
/*   Updated: 2016/02/26 15:13:33 by mbouhier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		special_permission(struct stat *file)
{
	((file->st_mode & S_IRUSR) == S_IRUSR) ? ft_putchar('r') : ft_putchar('-');
	((file->st_mode & S_IWUSR) == S_IWUSR) ? ft_putchar('w') : ft_putchar('-');
	if ((file->st_mode & S_ISUID) == S_ISUID)
		((file->st_mode & S_IXUSR) == S_IXUSR) ? ft_putchar('s') : ft_putchar('S');
	else if ((file->st_mode & S_IXUSR) == S_IXUSR)
		ft_putchar('x');
	else
		ft_putchar('-');
	((file->st_mode & S_IRGRP) == S_IRGRP) ? ft_putchar('r') : ft_putchar('-');
	((file->st_mode & S_IWGRP) == S_IWGRP) ? ft_putchar('w') : ft_putchar('-');
	if ((file->st_mode & S_ISGID) == S_ISGID)
		((file->st_mode & S_IXGRP) == S_IXGRP) ? ft_putchar('s') : ft_putchar('S');
	else if ((file->st_mode & S_IXUSR) == S_IXUSR)
		ft_putchar('x');
	else
		ft_putchar('-');
	((file->st_mode & S_IROTH) == S_IROTH) ? ft_putchar('r') : ft_putchar('-');
	((file->st_mode & S_IWOTH) == S_IWOTH) ? ft_putchar('w') : ft_putchar('-');
	if ((file->st_mode & S_ISVTX) == S_ISVTX)
		((file->st_mode & S_IXUSR) == S_IXUSR) ? ft_putchar('t') : ft_putchar('T');
	else if ((file->st_mode & S_IXUSR) == S_IXUSR)
		ft_putchar('x');
	else
		ft_putchar('-');
}

void		permission(struct stat *file)
{
	if ((S_IFMT & file->st_mode) == S_IFREG)
		ft_putchar('-');
	else if ((S_IFMT & file->st_mode) == S_IFDIR)
		ft_putchar('d');
	else if ((S_IFMT & file->st_mode) == S_IFCHR)
		ft_putchar('c');
	else if ((S_IFMT & file->st_mode) == S_IFBLK)
		ft_putchar('b');
	else if ((S_IFMT & file->st_mode) == S_IFIFO)
		ft_putchar('p');
	else if ((S_IFMT & file->st_mode) == S_IFLNK)
		ft_putchar('l');
	else if ((S_IFMT & file->st_mode) == S_IFSOCK)
		ft_putchar('s');
	else
		ft_putchar('?');
	special_permission(file);
}

void		color(t_info *list)
{
	if (S_ISDIR(list->file_stat.st_mode))
		ft_putstr("\033[5;36m");
	else if (S_ISSOCK(list->file_stat.st_mode))
		ft_putstr("\033[32m");
	else if (S_ISLNK(list->file_stat.st_mode))
		ft_putstr("\033[35m");
	else if (list->file_stat.st_mode & S_IXUSR)
		ft_putstr("\033[31m");
}

char		*ft_j(char *path, char *dir_name)
{
	int     len;
	char    *join;

	join = NULL;
	if (path && dir_name)
	{
		len = ft_strlen(path) + ft_strlen(dir_name);
		if (!(join = ft_strnew(len + 2)))
			error_param(2, 1, NULL);
		join = ft_strcpy(join, path);
		join = ft_strcat(join, "/");
		join = ft_strcat(join, dir_name);
	}
	return (join);
}

t_bool		is_hidden_file(char *file_path)
{
	return (file_path && file_path[0] == '.');
}

void		display_time(char *time)
{
	int				i;

	i = 4;
	while (i < 16)
		ft_putchar(time[i++]);
}

static int 	ft_len_nb(int nb)
{
	int count;

	count = 1;
	while (nb / 10 > 0)
	{
		nb = nb / 10;
		count++;
	} 
	return (count);
}

void 		size_display(t_env *e, t_info *list)
{
	struct group	*gp;
	struct passwd	*pass;

	gp = getgrgid(list->file_stat.st_gid);
	pass = getpwuid(list->file_stat.st_uid);
	e->link = ft_len_nb(list->file_stat.st_nlink) > e->link ?
	ft_len_nb(list->file_stat.st_nlink) : e->link;
	e->uid = ft_strlen(pass->pw_name) > e->uid ? ft_strlen(pass->pw_name) :
	e->uid;
	e->gid = ft_strlen(gp->gr_name) > e->gid ? ft_strlen(gp->gr_name) :
	e->gid;
	e->s_block = ft_len_nb(list->file_stat.st_size) > e->s_block ?
	ft_len_nb(list->file_stat.st_size) : e->s_block;
	e->minor_max = ft_len_nb(e->minor) > e->minor_max ?
	ft_len_nb(e->minor) : e->minor_max;
	e->major_max = ft_len_nb(e->major) > e->major_max ?
	ft_len_nb(e->major) : e->major_max;
	e->time_s = ft_strlen(ctime(&list->file_stat.st_mtime)) > e->time_s ?
	ft_strlen(ctime(&list->file_stat.st_mtime)) : e->time_s;
}

t_info 		*add_item(char *path, t_env *e, char *name)
{
	t_info    *list;

	if (!(list = (t_info*)malloc(sizeof(t_info))))
		error_param(2, 1, NULL);
	list->next = NULL;
	list->statut = 0;
	list->name = NULL;
	if (lstat(ft_j(path, name),	&list->file_stat) == -1)
	{
		perror(0);
		return (NULL);
	}
	if (!(list->name = ft_strdup(name)))
		error_param(2, 1, NULL);
	if (e->uppercase_r && S_ISDIR(list->file_stat.st_mode) 
		&& (ft_strcmp(list->name, ".") && ft_strcmp(list->name, "..")))
		list->statut = 1;
	size_display(e, list);
	return (list);
}

char	*last_word(char *str, char c)
{
	char	*tmp;
	int 	i;
	int 	j;
	int 	k;

	i = ft_strlen(str) - 1;
	k = i;
	j = 0;
	while (str[i] != c)
		--i;
	if (!(tmp = (char*)malloc(sizeof(char) * (k - i + 1))))
		error_param(2, 1, NULL);
	while (str[++i])
		tmp[j++] = str[i];
	tmp[j] = 0;
	return (tmp);
}

void	read_files(char *path, t_env *e, t_info **begin, int *blocs)
{
	t_info			*to_add;
	struct dirent	*data;
	DIR 			*rep;
	int 			i;

	to_add = NULL;
	i = -1;
	if (!(rep = opendir(path)))
		return (error_param(3, 3, last_word(path, '/')));
	while ((data = readdir(rep)))
	{
		if (data->d_name[0] == '.' && !e->a)
			continue ;
		to_add = add_item(path, e, data->d_name);
		if (to_add)
			*blocs += to_add->file_stat.st_blocks;
		if (++i == 0)
			*begin = to_add;
		else
			sort(e, to_add, begin);
	}
	closedir(rep);
}

void		read_dir(char *path, t_env *e, t_info *begin)
{
	char	*tmp;

	while (begin)
	{
		if (begin->statut == 1)
		{
			tmp = ft_j(path, begin->name);
			add_and_sort(tmp, e, -1);
		}
		begin = begin->next;
	}
}

void		add_space_str(int len)
{
	int		i;

	i = -1;
	while (++i < len)
		write(1, " ", 1);
}

void		add_space_nb(int len)
{
	int		i;

	i = -1;
	while (++i < len)
		write(1, " ", 1);
}

void		print_link(t_env *e, t_info *list, char *path)
{
	ft_putstr(list->name);
	ft_putstr("\033[0m -> ");
	e->s_link = ft_strnew(255);
	readlink(ft_j(path, list->name), e->s_link, 255);
	ft_putendl(e->s_link);
	free(e->s_link);
}

void		print_date(char *str)
{
	int				i;

	i = 3;
	while (++i < 11)
		ft_putchar(str[i]);
	i = 19;
	ft_putchar(' ');
	while (++i < 24)
		ft_putchar(str[i]);
}

void		print_file_l(t_env *e, t_info *begin, char *times, char *path)
{
	struct group	*gp;
	struct passwd	*pass;
	int i;

	e->major = begin->file_stat.st_rdev;
	gp = getgrgid(begin->file_stat.st_gid);
	pass = getpwuid(begin->file_stat.st_uid);
	i = begin->file_stat.st_mtime;
	times = ctime(&begin->file_stat.st_mtime);
	permission(&begin->file_stat);
	add_space_nb(e->link - ft_len_nb(begin->file_stat.st_nlink) + 2);
	ft_putnbr((int)begin->file_stat.st_nlink);
	ft_putchar(' ');
	if (!e->lowercase_g)
	{
		ft_putstr(pass->pw_name);
		add_space_str(e->uid - ft_strlen(pass->pw_name) + 2);
	}
	ft_putstr(gp->gr_name);
	add_space_str(e->gid - ft_strlen(gp->gr_name) + 2);
	if (path[0] == '/' && path[1] == 'd' && path[2] == 'e' && path[3] == 'v' && (path[4] == '\0' || path[4] == '/'))
	{
		while ((e->major / 256) > 0)
			e->major /= 256;
		e->minor = begin->file_stat.st_rdev % 256;
		size_display(e, begin);
		add_space_nb(e->major_max - ft_len_nb(e->major));
		if (S_ISBLK(begin->file_stat.st_mode) || S_ISREG(begin->file_stat.st_mode) || S_ISCHR(begin->file_stat.st_mode))
		{
			ft_putnbr(e->major);
			ft_putstr(", ");
			add_space_nb(e->minor_max - ft_len_nb(e->minor));
			ft_putnbr(e->minor);
		}
		else
		{
			add_space_nb(e->major_max + e->minor_max);
			ft_putnbr(e->minor);
		}
	}
	else
	{
		add_space_nb(e->s_block - ft_len_nb(begin->file_stat.st_size));
		ft_putnbr(begin->file_stat.st_size);
	}
	ft_putchar(' ');
	(time(NULL) - 60 * 60 * 24 * 180 > i || time(NULL) < i) ?
	print_date(times) : display_time(times);
	ft_putchar(' ');
	if (e->uppercase_g)
		color(begin);
	S_ISLNK(begin->file_stat.st_mode) ?
	print_link(e, begin, path) : ft_putendl(begin->name);
	ft_putstr("\033[0m");
}

int			test_open(char *path)
{
	DIR 	*rep;

	rep = opendir(path);
	if (rep)
		closedir(rep);
	else
		return (0);
	return (1);
}

void 		print_total(int blocs)
{
			ft_putstr("Total : ");
			ft_putnbr(blocs);
			ft_putchar('\n');
}

void 		print_files(t_env *e, t_info *begin, char *path, int blocs)
{
	if (begin)
	{
		if (e->l)
			print_total(blocs);
		if (!e->l)
			while (begin)
			{
				if (e->uppercase_g)
					color(begin);
				ft_putstr(begin->name);
				ft_putstr("\033[0m");
				begin = begin->next;
				if (begin)
					ft_putstr("   ");
				else
					ft_putchar('\n');
			}
		else
			while (begin)
			{
				print_file_l(e, begin, NULL, path);
				begin = begin->next;
			}
	}
}

void		print_name_dir(char *path, t_env *e, int type)
{
	if (e->uppercase_r && type == -1)
	{
		ft_putchar('\n');
		if (e->uppercase_r && ft_strcmp(path, ".") && ft_strcmp(path, "./"))
		{
			ft_putstr(path);
			ft_putstr(":\n");
		}
	}
}

void		add_and_sort(char *path, t_env *e, int type)
{
	t_info	*begin;
	int 	blocs;

	blocs = 0;
	begin = NULL;
	print_name_dir(path, e, type);
	if (path )
	read_files(path, e, &begin, &blocs);
	print_files(e, begin, path, blocs);
	if (e->uppercase_r)
		read_dir(path, e, begin);
}
