#include "test.h"

/* function a tester*/
int		ft_list_size(t_list *begin_list);

int list_size(t_list *begin_list)
{
	int counter;

	counter = 0;
	while (begin_list != NULL)
	{
		counter++;
		begin_list = begin_list->next;
	}
	return counter;
}

struct test_struct {
    char                    *test;
    int                     expected;
    int                     answer;
    int                     segfault;
    struct test_struct    *next;
};
static struct test_struct *failure = NULL;

static int TEST_SEGFAULT(t_list *tmp) {
    int pid = 0;

    if ((pid = fork()) < 0)
		exit(EXIT_FAILURE);
	if (pid == 0) {
        ft_list_size(tmp);
		exit(EXIT_SUCCESS);
    }
    wait(&pid);
    return (WIFSIGNALED(pid));
}

static int counter = -1;
static void FT_LIST_SIZE_EXPECT(char *str) {
    counter++;
    struct test_struct *test;
    t_list *tmp;
    if (counter % 10 == 0 && counter)
        dprintf(1,"\n\t\t\t");

    test = malloc(sizeof(struct test_struct));
	tmp = list_from_format(str);
    test->expected = list_size(tmp);
    test->answer = 0;
    test->test = str;
    test->next = NULL;
    test->segfault = TEST_SEGFAULT(tmp);
    if (!test->segfault)
        test->answer = ft_list_size(tmp);
    list_destroy(tmp);
    test->segfault ? SEGFAULT : test->answer != test->expected ? KO : OK;
    if (test->segfault || test->answer != test->expected) {
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


static void ft_list_size_error() {
    struct test_struct *tmp = failure;
    struct test_struct *next;

    while (tmp) {
        next = tmp->next;
        dprintf(detail_fd, "\n\n\t%s:\033[37;3m TEST:       ft_list_size(\"%s\")\033[0m\n", tmp->segfault ? "\033[31;1msegfault" : "\033[33;3mfailure", str_escape(tmp->test));
        dprintf(detail_fd, "\t\033[0;33m        \033[31;3m ANSWER:     %-5d\033[0m\n", tmp->answer);
        dprintf(detail_fd, "\t\033[0;33m        \033[32;3m EXPECTED:   %-5d\033[0m\n", tmp->expected);
        free(tmp);
        tmp = next;
    }
}

static int ft_list_size_test_list() {
	FT_LIST_SIZE_EXPECT("1 2 3");
	FT_LIST_SIZE_EXPECT("");
	FT_LIST_SIZE_EXPECT("1 2 3 4 10");
	FT_LIST_SIZE_EXPECT("19879 123 12344");
	FT_LIST_SIZE_EXPECT("1");
	FT_LIST_SIZE_EXPECT("1 2");
	FT_LIST_SIZE_EXPECT(NULL);
	FT_LIST_SIZE_EXPECT("9 3 123 4141 23523 32 13");
	FT_LIST_SIZE_EXPECT("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20");
	FT_LIST_SIZE_EXPECT("20 19 18 17 16 15 14 13 12 11 10 9 87 6 5 4 3 2 1 0");

    return (failure != NULL);
}

void ft_list_size_test() {tester("ft_list_size", &ft_list_size_test_list, &ft_list_size_error);};