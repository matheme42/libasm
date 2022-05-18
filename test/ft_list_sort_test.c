#include "test.h"

/* function a tester*/
void ft_list_sort(t_list **begin_list, int (*cmp)(void *, void*));

static t_list* merge_sorted_list(t_list* l1, t_list* l2, int (*cmp)(void *, void*))
{
	t_list *merged = 0x0;

	if (l1 == 0x0)
		return l2;
	if (l2 == 0x0)
		return l1;
	if (cmp(l1->data, l2->data) < 0)
	{
		merged = l1;
		merged->next = merge_sorted_list(l1->next, l2, cmp);
	}
	else
	{
		merged = l2;
		merged->next = merge_sorted_list(l1, l2->next, cmp);
	}
	return merged;
}

void list_sort(t_list **begin_list, int (*cmp)(void *, void*))
{
	if (begin_list == 0x0 || *begin_list == 0x0
			|| (*begin_list)->next == 0x0)
		return ;
	t_list *fast = (*begin_list)->next;
	t_list *slow = *begin_list;
	while (fast != 0x0)
	{
		fast = fast->next;
		if (fast != 0x0)
		{
			fast = fast->next;
			slow = slow->next;
		}
	}
	t_list *middle = slow->next;
	slow->next = 0x0;

	list_sort(begin_list, cmp);
	list_sort(&middle, cmp);
	*begin_list = merge_sorted_list(*begin_list, middle, cmp);
}


struct test_struct {
    char                    *test;
    t_list *                expected;
    t_list *                answer;
    int                     segfault;
    struct test_struct      *next;
};
static struct test_struct *failure = NULL;

static int compar_int(void *a, void *b)
{
	return *(int*)a - *(int*)b;
}


static int TEST_SEGFAULT(t_list *begin) {
    int pid = 0;

    if ((pid = fork()) < 0)
		exit(EXIT_FAILURE);
	if (pid == 0) {
        ft_list_sort(&begin, &compar_int);
		exit(EXIT_SUCCESS);
    }
    wait(&pid);
    return (WIFSIGNALED(pid));
}

static int counter = -1;
static void FT_LIST_SORT_EXPECT(char *str) {
    counter++;
    struct test_struct *test;
    t_list *list_segfault;
    if (counter % 10 == 0 && counter)
        dprintf(1,"\n\t\t\t");

    test = malloc(sizeof(struct test_struct));
	test->expected = list_from_format(str);
	test->answer = list_from_format(str);
	list_segfault = list_from_format(str);
    list_sort(&test->expected, &compar_int);  
    test->test = str;
    test->next = NULL;
    test->segfault = TEST_SEGFAULT(list_segfault);
    if (!test->segfault)
        ft_list_sort(&test->answer, &compar_int);
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


static void ft_list_sort_error() {
    struct test_struct *tmp = failure;
    struct test_struct *next;

    while (tmp) {
        next = tmp->next;
        dprintf(detail_fd, "\n\n\t%s:\033[37;3m TEST:       ft_list_sort(\"%s\")\033[0m\n", tmp->segfault ? "\033[31;1msegfault" : "\033[33;3mfailure", str_escape(tmp->test));
        dprintf(detail_fd, "\t\033[0;33m        \033[31;3m ANSWER:   "); list_print(tmp->answer);  dprintf(1, "\033[0m\n");
        dprintf(detail_fd, "\t\033[0;33m        \033[32;3m EXPECTED: "); list_print(tmp->expected);  dprintf(1, "\033[0m\n");
        free(tmp);
		tmp = next;
    }
}

static int ft_list_sort_test_list() {
	FT_LIST_SORT_EXPECT("");
	FT_LIST_SORT_EXPECT("0");
	FT_LIST_SORT_EXPECT("1");
	FT_LIST_SORT_EXPECT("2 1");
	FT_LIST_SORT_EXPECT("1 2 3");
	FT_LIST_SORT_EXPECT("-1 0 1 2 3 4 5 6");
	FT_LIST_SORT_EXPECT("98 12 12 45 1 -1 232 34 23");
	FT_LIST_SORT_EXPECT("12 45 1 -1 232 34 23 87879");
	FT_LIST_SORT_EXPECT("1 2 3");
	FT_LIST_SORT_EXPECT("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 0 20");
    return (failure != NULL);
}

void ft_list_sort_test() {tester("ft_list_sort", &ft_list_sort_test_list, &ft_list_sort_error);};