/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: t <t@student.42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 01:03:24 by t                 #+#    #+#             */
/*   Updated: 2022/05/18 12:25:17 by t                ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_LIBASM_H
#define TEST_LIBASM_H

#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

#define KO dprintf(1, " \033[0;31m[KO]\033[0m")
#define OK dprintf(1, " \033[0;32m[OK]\033[0m")
#define SEGFAULT dprintf(1, " \033[31;1m[SEGFAULT]\033[0m")

typedef struct s_list
{
	void *data;
	struct s_list *next;
} t_list;

extern int detail_fd;
extern char *test_function;

char    *str_escape(char str[]);
void    tester(char *s, int test(), void error());
t_list  *list_from_format(char *fmt);
int     *create_data_elem(int data);
int     list_cmp(t_list *l1, t_list *l2);
void    list_destroy(t_list *list);
void    list_print(t_list *list);

void    ft_strlen_test();
void    ft_strcmp_test();
void    ft_strcpy_test();
void    ft_strdup_test();
void    ft_write_test();
void    ft_read_test();
void    ft_atoi_base_test();
void    ft_list_size_test();
void    ft_list_push_front_test();
void	ft_list_sort_test();
void	ft_list_remove_if_test();
void	ft_bzero_test();
void	ft_putnbr_test();
#endif