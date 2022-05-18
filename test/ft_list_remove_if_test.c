#include "test.h"

/* function a tester*/
void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(void *, void *), void (free_fct)(void *));

void list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (free_fct)(void *))
{
	t_list *saved_next;

	if (begin_list == NULL || *begin_list == NULL)
		return ;
	if (cmp((*begin_list)->data, data_ref) != 0)
	{
		list_remove_if(&(*begin_list)->next, data_ref, cmp, free_fct);
		return ;
	}
	saved_next = (*begin_list)->next;
	free_fct((*begin_list)->data);
	free(*begin_list);
	*begin_list = saved_next;
	list_remove_if(begin_list, data_ref, cmp, free_fct);
}


static int compar(void *a, void *ref)
{
	return *(int*)a - *(int*)ref;
}

static void free_fct(void *data)
{
	free(data);
}

struct test_struct {
    char                    *test;
    t_list *                expected;
    t_list *                answer;
    int                     segfault;
    struct test_struct    *next;
};
static struct test_struct *failure = NULL;

static int TEST_SEGFAULT(t_list *begin, void *data) {
    int pid = 0;

    if ((pid = fork()) < 0)
		exit(EXIT_FAILURE);
	if (pid == 0) {
        ft_list_remove_if(&begin, &data, &compar, &free_fct);
		exit(EXIT_SUCCESS);
    }
    wait(&pid);
    return (WIFSIGNALED(pid));
}

static int counter = -1;
static void FT_LIST_REMOVE_IF_EXPECT(char *str, int data) {
    counter++;
    struct test_struct *test;
    t_list *list_segfault;
    if (counter % 10 == 0 && counter)
        dprintf(1,"\n\t\t\t");

    test = malloc(sizeof(struct test_struct));
	test->expected = list_from_format(str);
	test->answer = list_from_format(str);
	list_segfault = list_from_format(str);
    list_remove_if(&test->expected,  &data, &compar, &free_fct);
    test->test = str;
    test->next = NULL;
    test->segfault = TEST_SEGFAULT(list_segfault, &data);
    if (!test->segfault)
        ft_list_remove_if(&test->answer,  &data, &compar, &free_fct);
    test->segfault ? SEGFAULT : list_cmp(test->answer,test->expected) ? KO : OK;
    if (test->segfault || list_cmp(test->answer, test->expected)) {
        if (!failure) {
            failure = test;
            return ;
        } 
        struct test_struct *tmp = failure;
        while(tmp->next) tmp = tmp->next;
        tmp->next = test;
        return ;
    }
    free(test);
}


static void ft_list_remove_if_error() {
    struct test_struct *tmp = failure;
    struct test_struct *next;

    while (tmp) {
        next = tmp->next;
        dprintf(detail_fd, "\n\n\t%s:\033[37;3m TEST:       ft_list_remove_if(\"%s\")\033[0m\n", tmp->segfault ? "\033[31;1msegfault" : "\033[33;3mfailure", str_escape(tmp->test));
        dprintf(detail_fd, "\t\033[0;33m        \033[31;3m ANSWER:   "); list_print(tmp->answer);  dprintf(1, "\033[0m\n");
        dprintf(detail_fd, "\t\033[0;33m        \033[32;3m EXPECTED: "); list_print(tmp->expected);  dprintf(1, "\033[0m\n");
        free(tmp);
        tmp = next;
    }
}

static int ft_list_remove_if_test_list() {
	FT_LIST_REMOVE_IF_EXPECT("", 0);
	FT_LIST_REMOVE_IF_EXPECT("1 2", 3);
	FT_LIST_REMOVE_IF_EXPECT("1 2 3", 3);
	FT_LIST_REMOVE_IF_EXPECT("1 2 3", 1);
	FT_LIST_REMOVE_IF_EXPECT("1 2 3", 2);
	FT_LIST_REMOVE_IF_EXPECT("987 3 9812 3 12 312 3 12491234989 3 908 3", 3);
	FT_LIST_REMOVE_IF_EXPECT("1 1 1 1 1 1 1 1", 1);
	FT_LIST_REMOVE_IF_EXPECT("1 2 1 2 1 2 1 2", 1);
	FT_LIST_REMOVE_IF_EXPECT("1 2 1 2 1 2 1 2", 2);
	FT_LIST_REMOVE_IF_EXPECT("3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2", 7);

    return (failure != NULL);
}

void ft_list_remove_if_test() {tester("ft_list_remove_if", &ft_list_remove_if_test_list, &ft_list_remove_if_error);};