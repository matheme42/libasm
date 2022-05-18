#include "test.h"

/* function a tester*/
int		ft_strcmp(char *s1, char *s2);



struct test_struct {
    char                    *s1;
    char                    *s2;
    int                     expected;
    int                     answer;
    int                     segfault;
    struct test_struct    *next;
};

static struct test_struct *failure = NULL;

static int TEST_SEGFAULT(char *s1, char *s2) {
    int pid = 0;

    if ((pid = fork()) < 0)
		exit(EXIT_FAILURE);
	if (pid == 0) {
        ft_strcmp(s1, s2);
		exit(EXIT_SUCCESS);
    }
    wait(&pid);
    return (WIFSIGNALED(pid));
}

static int counter = -1;
static void FT_STRCMP_EXPECT(char *s1, char *s2) {
    counter++;
    struct test_struct *test;
    if (counter % 10 == 0 && counter)
        dprintf(1,"\n\t\t\t");

    test = malloc(sizeof(struct test_struct));
    test->expected = strcmp(s1, s2);
    test->answer = 0;
    test->s1 = s1;
    test->s2 = s2;


    test->next = NULL;
    test->segfault = TEST_SEGFAULT(s1, s2);
    if (!test->segfault)
        test->answer = ft_strcmp(s1, s2);
    test->segfault ? SEGFAULT : test->answer != test->expected ? KO : OK;
    if (test->answer != test->expected || test->segfault) {
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


static void ft_strcmp_error() {
    struct test_struct *tmp = failure;
    struct test_struct *next;
    while (tmp) {
        next = tmp->next;
        dprintf(detail_fd, "\n\n\t%s:\033[37;3m TEST:       ft_strcmp(\"%s\", \"%s\")\033[0m\n", tmp->segfault ? "\033[31;1msegfault" : "\033[33;3mfailure", str_escape(tmp->s1), str_escape(tmp->s2));
        dprintf(detail_fd, "\t\033[0;33m        \033[31;3m ANSWER:     %-5d\033[0m\n", tmp->answer);
        dprintf(detail_fd, "\t\033[0;33m        \033[32;3m EXPECTED:   %-5d\033[0m\n", tmp->expected);
        free(tmp);
        tmp = next;
    }
}

static int ft_strcmp_test_list() {
	FT_STRCMP_EXPECT("", "");
	FT_STRCMP_EXPECT("bon", "");
	FT_STRCMP_EXPECT("bonjour", "");
	FT_STRCMP_EXPECT("asdklfjasdfj////asdf'''asdf3##", "");
	FT_STRCMP_EXPECT("the\0hidden", "");
	FT_STRCMP_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.", "");
	FT_STRCMP_EXPECT("", "");
	FT_STRCMP_EXPECT("bon", "bon");
	FT_STRCMP_EXPECT("bonjour", "bonjour");
	FT_STRCMP_EXPECT("asdklfjasdfj////asdf'''asdf3##", "asdklfjasdfj////asdf'''asdf3##");
	FT_STRCMP_EXPECT("the\0hidden", "the\0hidden");
	FT_STRCMP_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.",
"Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.");

	FT_STRCMP_EXPECT("", "asdf");
	FT_STRCMP_EXPECT("bon", "bo");
	FT_STRCMP_EXPECT("bonjour", "onjour");
	FT_STRCMP_EXPECT("asdklfjasdfj////asdf'''asdf3##", "asdklfjasdfj////asdf'''asdf3##");
	FT_STRCMP_EXPECT("the\0hidden", "thehidden");
	FT_STRCMP_EXPECT("Lorem ipsum dolor st amet, consectetur adipiscing", "Lodsfsdfasdf");
	FT_STRCMP_EXPECT("b", "b");
	FT_STRCMP_EXPECT("bb", "b");
	FT_STRCMP_EXPECT("b\n", "b\n");
	FT_STRCMP_EXPECT("baraevffzaczec", "baraevffzaczec");
	FT_STRCMP_EXPECT("\x01", "\x01");
	FT_STRCMP_EXPECT("\x02", "\x01");
	FT_STRCMP_EXPECT("\x01", "\x02");
	FT_STRCMP_EXPECT("\xff", "\xff");
	FT_STRCMP_EXPECT("\xfe", "\xff");
	FT_STRCMP_EXPECT("\xff", "\xfe");

	FT_STRCMP_EXPECT("\x01\x01", "\x01");
	FT_STRCMP_EXPECT("\x01\x02", "\x01");
	FT_STRCMP_EXPECT("\x02\x01", "\x02");
	FT_STRCMP_EXPECT("\xff\xff", "\xff");
	FT_STRCMP_EXPECT("\xff\xfe", "\xff");
	FT_STRCMP_EXPECT("\xfe\xff", "\xfe");

	FT_STRCMP_EXPECT("\x01", "\x01\x01");
	FT_STRCMP_EXPECT("\x01", "\x01\x02");
	FT_STRCMP_EXPECT("\x02", "\x02\x01");
	FT_STRCMP_EXPECT("\xff", "\xff\xff");
	FT_STRCMP_EXPECT("\xff", "\xff\xfe");
	FT_STRCMP_EXPECT("\xfe", "\xfe\xff");
    return (failure != NULL);
}

void ft_strcmp_test() {tester("ft_strcmp", &ft_strcmp_test_list, &ft_strcmp_error);};