#include "test.h"

/* function a tester*/
void ft_list_push_front(t_list **begin_list, void *data);

void list_push_front(t_list **begin_list, void *data)
{
	if (begin_list == NULL)
		return ;
	t_list *new = malloc(sizeof(t_list));
	if (new == NULL)
		return ;
	new->data = data;
	new->next = *begin_list;
	*begin_list = new;
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
        ft_list_push_front(&begin, data);
		exit(EXIT_SUCCESS);
    }
    wait(&pid);
    return (WIFSIGNALED(pid));
}

static int counter = -1;
static void FT_LIST_PUSH_FRONT_EXPECT(char *str, int data) {
    counter++;
    struct test_struct *test;
    t_list *list_segfault;
    if (counter % 10 == 0 && counter)
        dprintf(1,"\n\t\t\t");

    test = malloc(sizeof(struct test_struct));
	test->expected = list_from_format(str);
	test->answer = list_from_format(str);
	list_segfault = list_from_format(str);
    list_push_front(&test->expected, &data);
    test->test = str;
    test->next = NULL;
    test->segfault = TEST_SEGFAULT(list_segfault, &data);
    if (!test->segfault)
        ft_list_push_front(&test->answer, &data);
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


static void ft_list_push_front_error() {
    struct test_struct *tmp = failure;
    struct test_struct *next;

    while (tmp) {
        next = tmp->next;
        dprintf(detail_fd, "\n\n\t%s:\033[37;3m TEST:       ft_list_push_front(\"%s\")\033[0m\n", tmp->segfault ? "\033[31;1msegfault" : "\033[33;3mfailure", str_escape(tmp->test));
        dprintf(detail_fd, "\t\033[0;33m        \033[31;3m ANSWER:   "); list_print(tmp->answer);  dprintf(1, "\033[0m\n");
        dprintf(detail_fd, "\t\033[0;33m        \033[32;3m EXPECTED: "); list_print(tmp->expected);  dprintf(1, "\033[0m\n");
        free(tmp);
        tmp = next;
    }
}

static int ft_list_push_front_test_list() {
	FT_LIST_PUSH_FRONT_EXPECT("", 0);
	FT_LIST_PUSH_FRONT_EXPECT("", 1);
	FT_LIST_PUSH_FRONT_EXPECT("0", 1);
	FT_LIST_PUSH_FRONT_EXPECT("1 2 3", 4);
	FT_LIST_PUSH_FRONT_EXPECT("1 2 3", -1);
	FT_LIST_PUSH_FRONT_EXPECT("0 9 2 1", 11239);
	FT_LIST_PUSH_FRONT_EXPECT("1 2 3 4 7 7", 7);
	FT_LIST_PUSH_FRONT_EXPECT(NULL, 7);
	FT_LIST_PUSH_FRONT_EXPECT("1 1 1 1 1 1 1 1 1 1 1 1 1 1", 1);
	FT_LIST_PUSH_FRONT_EXPECT("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20", 7);
    return (failure != NULL);
}

void ft_list_push_front_test() {tester("ft_list_push_front", &ft_list_push_front_test_list, &ft_list_push_front_error);};