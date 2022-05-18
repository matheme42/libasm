#include "test.h"

/* function a tester*/
void		ft_bzero(const char *s, int size);



struct test_struct {
    char                    *test;
    char                    *expected;
    char                    *answer;
    int                     segfault;
    struct test_struct      *next;
};
static struct test_struct *failure = NULL;

static int TEST_SEGFAULT(char *s, int size) {
    int pid = 0;

    if ((pid = fork()) < 0)
		exit(EXIT_FAILURE);
	if (pid == 0) {
        ft_bzero(s, size);
        free(s);
		exit(EXIT_SUCCESS);
    }
    wait(&pid);
    return (WIFSIGNALED(pid));
}

static int counter = -1;
static void FT_BZERO_EXPECT(char *str, int size) {
    counter++;
    struct test_struct *test;
    if (counter % 10 == 0 && counter)
        dprintf(1,"\n\t\t\t");

    test = malloc(sizeof(struct test_struct));
    test->expected =  strdup(str);
    bzero(test->expected, size);
    test->answer = 0;
    test->test = str;
    test->next = NULL;
    test->segfault = TEST_SEGFAULT(strdup(str), size);
    if (!test->segfault) {
        test->answer = strdup(str);
        ft_bzero(test->answer, size);
    }
    test->segfault ? SEGFAULT : memcmp(test->answer,test->expected, size) ? KO : OK;
    if (test->segfault || memcmp(test->answer, test->expected, size)) {
        if (!failure) {
            failure = test;
            return ;
        } 
        struct test_struct *tmp = failure;
        while(tmp->next) tmp = tmp->next;
        tmp->next = test;
        return ;
    }
    free(test->answer);
    free(test->expected);
    free(test);
}


static void ft_bzero_error() {
    struct test_struct *tmp = failure;
    struct test_struct *next;

    while (tmp) {
        next = tmp->next;
        dprintf(detail_fd, "\n\n\t%s:\033[37;3m TEST:       ft_bzero(\"%s\")\033[0m\n", tmp->segfault ? "\033[31;1msegfault" : "\033[33;3mfailure", str_escape(tmp->test));
        dprintf(detail_fd, "\t\033[0;33m        \033[31;3m ANSWER:  \033[0m\n");
        dprintf(detail_fd, "\t\033[0;33m        \033[32;3m EXPECTED:\033[0m\n");
        free(tmp->answer);
        free(tmp->expected);
        free(tmp->test);
        free(tmp);
        tmp = next;
    }
}

static int ft_bzero_test_list() {
    FT_BZERO_EXPECT("", 0);
    FT_BZERO_EXPECT("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0d", 15);
    FT_BZERO_EXPECT("qsd", 0);
    FT_BZERO_EXPECT("azerty", 5);
    FT_BZERO_EXPECT("azertty\0", 0);
    FT_BZERO_EXPECT("\n\t\n\v\f   \0", 4);
    FT_BZERO_EXPECT("\n\t\n\v\fazeaze   \0csdfqzf012385qsvf", 20);
    FT_BZERO_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.", 50);
    FT_BZERO_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.", 200);
    FT_BZERO_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.", 0);
    return (failure != NULL);
}

void ft_bzero_test() {tester("ft_bzero", &ft_bzero_test_list, &ft_bzero_error);};