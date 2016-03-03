/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouhier <mbouhier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/21 22:01:21 by maboukra          #+#    #+#             */
/*   Updated: 2016/02/29 00:15:18 by mbouhier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/types.h>
# include <grp.h>
# include <pwd.h>
# include <time.h>
# include <uuid/uuid.h>
# include <stdio.h>
# include "includes/libft.h"

# define CHECK_FLAGS "RatrlGg"

typedef enum		e_bool
{
	FALSE,
	TRUE
}					t_bool;

typedef struct				s_info
{
	char					*path;
	char 					*name;
	int 					statut;
	struct stat				file_stat;
	struct s_info			*next;
	struct s_info			*down;
	int 					blocs;
}							t_info;

typedef struct				s_env
{
	char					**path;
	char					**files;
	char					uppercase_g;
	char					uppercase_r;
	char					t;
	char					a;
	char					lowercase_r;
	char					lowercase_g;
	char					l;
	char					*tmp;
	int						nb_path;
	int 					nb_files;
	char					*full_path;
	char					*s_link;
	int 					link;
	size_t 					uid;
	size_t 					gid;
	int 					s_block;
	size_t 					time_s;
	dev_t					minor_max;
	dev_t					major_max;
	dev_t					minor;
	dev_t					major;
}							t_env;

t_env						*create_env(char **path);
t_info						*create_list(char *path, t_env *e, t_info **begin);
t_info   					*create_next_list(char *path, t_env *e,\
t_info *begin);
void						add_and_sort(char *path, t_env *e, int type);
void						push_down(char *path, t_env *e, \
t_info *list);
char						*ft_j(char *path, char *dir_name);
void						sort(t_env *e, t_info *to_add, t_info **begin);
void						sort_down(t_env *e, t_info *list, \
t_info *to_add);
void						parser(int ac, char **av, t_env *e, int i);
void						error_param(int type, int ex, char *str);
void						init_path(t_env *e);
int							fill_path(t_env *e, int j, char **av);
void						get_data(t_env *e);
void						display_files_l(t_env *e, t_info *begin, int i, int k);
void						permission(struct stat *file);
void 						size_display(t_env *e, t_info *list);
void						print_file_l(t_env *e, t_info *begin, char *time, char *path);
void						add_space_nb(int len);
void						add_space_str(int len);
void						print_date(char *str);
void						print_link(t_env *e, t_info *list, char *path);
void						display_time(char *time);
char						*ft_j(char *path, char *dir_name);
char						*last_word(char *str, char c);
void						read_dir(char *path, t_env *e, t_info *begin);
void						print_name_dir(char *path, t_env *e, int type);
void						read_files(char *path, t_env *e, t_info **begin, \
int *blocs);
void						print_files(t_env *e, t_info *begin, char *path, \
int blocs);
t_info						*add_item(char *path, t_env *e, char *name);
void						color(t_info *list);
void						init_var2(t_env *e);


#endif
