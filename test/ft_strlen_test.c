#include "test.h"

/* function a tester*/
int		ft_strlen(const char *s);



struct test_struct {
    char                    *test;
    int                     expected;
    int                     answer;
    int                     segfault;
    struct test_struct    *next;
};
static struct test_struct *failure = NULL;

static int TEST_SEGFAULT(char *s) {
    int pid = 0;

    if ((pid = fork()) < 0)
		exit(EXIT_FAILURE);
	if (pid == 0) {
        ft_strlen(s);
		exit(EXIT_SUCCESS);
    }
    wait(&pid);
    return (WIFSIGNALED(pid));
}

static int counter = -1;
static void FT_STRLEN_EXPECT(char *str) {
    counter++;
    struct test_struct *test;
    if (counter % 10 == 0 && counter)
        dprintf(1,"\n\t\t\t");

    test = malloc(sizeof(struct test_struct));
    test->expected = strlen(str);
    test->answer = 0;
    test->test = str;
    test->next = NULL;
    test->segfault = TEST_SEGFAULT(str);
    if (!test->segfault)
        test->answer = ft_strlen(str);
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


static void ft_strlen_error() {
    struct test_struct *tmp = failure;
    struct test_struct *next;

    while (tmp) {
        next = tmp->next;
        dprintf(detail_fd, "\n\n\t%s:\033[37;3m TEST:       ft_strlen(\"%s\")\033[0m\n", tmp->segfault ? "\033[31;1msegfault" : "\033[33;3mfailure", str_escape(tmp->test));
        dprintf(detail_fd, "\t\033[0;33m        \033[31;3m ANSWER:     %-5d\033[0m\n", tmp->answer);
        dprintf(detail_fd, "\t\033[0;33m        \033[32;3m EXPECTED:   %-5d\033[0m\n", tmp->expected);
        free(tmp);
        tmp = next;
    }
}

static int ft_strlen_test_list() {
    FT_STRLEN_EXPECT("");
    FT_STRLEN_EXPECT("azerty");
    FT_STRLEN_EXPECT("a");
    FT_STRLEN_EXPECT("11111111111111111111111111111111111111111111111111111&");
    FT_STRLEN_EXPECT("\0");
    FT_STRLEN_EXPECT("\n\t\n\v\f   \0");
    FT_STRLEN_EXPECT("\x01\x02\x03\x04\x05\0");
    FT_STRLEN_EXPECT("\n\t\n\v\fazeaze   \0csdfqzf012385qsvf");
    FT_STRLEN_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.");
    FT_STRLEN_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.");
    return (failure != NULL);
}

void ft_strlen_test() {tester("ft_strlen", &ft_strlen_test_list, &ft_strlen_error);};