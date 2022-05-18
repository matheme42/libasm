#include "test.h"

/* function a tester*/
char	*ft_strdup(const char *s);



struct test_struct {
    char                    *test;
    char                    *expected;
    char                    *answer;
    int                     segfault;
    struct test_struct    *next;
};
static struct test_struct *failure = NULL;

static int TEST_SEGFAULT(char *s) {
    int pid = 0;

    if ((pid = fork()) < 0)
		exit(EXIT_FAILURE);
	if (pid == 0) {
        char *b = ft_strdup(s);
        if (b == s && s != NULL)
            raise(SIGSEGV) ;
        if (b) free(b);
		exit(EXIT_SUCCESS);
    }
    wait(&pid);
    return (WIFSIGNALED(pid));
}

static int counter = -1;
static void FT_STRDUP_EXPECT(char *str, long sabote) {
    if (str == NULL)
        return ;
    counter++;
    struct test_struct *test;
    if (counter % 10 == 0 && counter)
        dprintf(1,"\n\t\t\t");


    test = malloc(sizeof(struct test_struct));

    if (sabote) {
        setrlimit(RLIMIT_AS, &(struct rlimit){0, RLIM_INFINITY});
        while (malloc(5) != NULL);
        while (malloc(1024) != NULL);
        while (malloc(500) != NULL);
    }
    test->expected = strdup(str);
    test->answer = 0;
    test->test = str;
    test->next = NULL;
    test->segfault = TEST_SEGFAULT(str);
    if (!test->segfault)
        test->answer = ft_strdup(str);
    setrlimit(RLIMIT_AS, &(struct rlimit){RLIM_INFINITY , RLIM_INFINITY});
    if (sabote && !test->segfault) {
        OK;
        free(test);
        return ;
    }
    test->segfault ? SEGFAULT : strcmp(test->answer,test->expected) ? KO : OK;
    if (test->segfault || strcmp(test->answer, test->expected)) {
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


static void ft_strdup_error() {
    struct test_struct *tmp = failure;
    struct test_struct *next;
    while (tmp) {
        next = tmp->next;
        dprintf(detail_fd, "\n\n\t%s:\033[37;3m TEST:       ft_strdup(\"%s\")\033[0m\n", tmp->segfault ? "\033[31;1msegfault" : "\033[33;3mfailure", str_escape(tmp->test));
        dprintf(detail_fd, "\t\033[0;33m        \033[31;3m ANSWER:     %s\033[0m\n", tmp->answer);
        dprintf(detail_fd, "\t\033[0;33m        \033[32;3m EXPECTED:   %s\033[0m\n", tmp->expected);
        free(tmp);
        tmp = next;
    }
}

static int ft_strdup_test_list() {
    FT_STRDUP_EXPECT("bonjour", 1);
    FT_STRDUP_EXPECT("", 0);
    FT_STRDUP_EXPECT("", 1);
    FT_STRDUP_EXPECT(NULL, 0);
    FT_STRDUP_EXPECT(NULL, 1);
    FT_STRDUP_EXPECT("azerty", 0);
    FT_STRDUP_EXPECT("azertyazertyazertyazertyazertyazertyazertyazertyazerty\
    azertyazertyazertyazertyazertyazertyazertyazertyazertyazertyazertyazerty\
    ", 0);
    FT_STRDUP_EXPECT("\0", 0);
    FT_STRDUP_EXPECT("\n\t\n\v\f   \0", 0);
    FT_STRDUP_EXPECT("\n\t\n\v\fazeaze   \0csdfqzf012385qsvf", 0);
    FT_STRDUP_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.", 0);
    FT_STRDUP_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.", 1);
    return (failure != NULL);
}

void ft_strdup_test() {tester("ft_strdup", &ft_strdup_test_list, &ft_strdup_error);};