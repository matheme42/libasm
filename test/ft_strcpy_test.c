#include "test.h"

/* function a tester*/
char		*ft_strcpy(char *dest, const char *src);



struct test_struct {
    char                    *test;
    char                    *expected;
    char                    *answer;
    int                     segfault;
    struct test_struct      *next;
};

static struct test_struct *failure = NULL;

static int TEST_SEGFAULT(char *dest, char *src) {
    int pid = 0;

    if ((pid = fork()) < 0)
		exit(EXIT_FAILURE);
	if (pid == 0) {
        ft_strcpy(dest, src);
		exit(EXIT_SUCCESS);
    }
    wait(&pid);
    return (WIFSIGNALED(pid));
}

static int counter = -1;
static void FT_STRCPY_EXPECT(char *s) {
    int len = strlen(s);
    counter++;
    struct test_struct *test;
    if (counter % 10 == 0 && counter)
        dprintf(1,"\n\t\t\t");

    char *buff = (char*)calloc(len + 1, 1);
    char *buff2 = (char*)calloc(len + 1, 1);
    for (int i = 0; i<= len; i++) {
        buff[i] = (i + 40) % 126;
        buff2[i] = (i + 40) % 126;
    }
    test = (struct test_struct*)malloc(sizeof(struct test_struct));

    test->expected = strcpy(buff, s);
    test->answer = 0;
    test->test = s;


    test->next = NULL;
    test->segfault = TEST_SEGFAULT(buff2, s);
    bzero(buff2, len);
    if (!test->segfault)
        test->answer = ft_strcpy(buff2, s);
    test->segfault ? SEGFAULT : strcmp(test->answer, test->expected) ? KO : OK;
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


static void ft_strcpy_error() {
    struct test_struct *tmp = failure;
    struct test_struct *next;

    while (tmp) {
        next = tmp->next;
        dprintf(detail_fd, "\n\n\t%s:\033[37;3m TEST:       ft_strcpy(\"dest\", \"%s\")\033[0m\n", tmp->segfault ? "\033[31;1msegfault" : "\033[33;3mfailure", str_escape(tmp->test));
        dprintf(detail_fd, "\t\033[0;33m        \033[31;3m ANSWER:     %s\033[0m\n", tmp->answer);
        dprintf(detail_fd, "\t\033[0;33m        \033[32;3m EXPECTED:   %s\033[0m\n", tmp->expected);
        free(tmp);
        tmp = next;
    }
}

static int ft_strcpy_test_list() {
	FT_STRCPY_EXPECT("");
	FT_STRCPY_EXPECT("abc");
	FT_STRCPY_EXPECT("asl;fjl;asdjfjkasdl;fjadjsf");
	FT_STRCPY_EXPECT("qsvzeefscseffwcqdsdcx");
	FT_STRCPY_EXPECT("A");
	FT_STRCPY_EXPECT("asl;fjl;asd ffsdcf<q<sf ascc<sh jvhazgJK<QBQZEJE<HGBCJAZVGEX Cjfjkasdl;fjadjsf\
    QSVEZQ< FSDKJVDWSJF HBZQKCJ<FHSGFDJWD?HQEEBKJF< GSHalbzj<shgchqsbkjcwsdgnjbcaG\
    ");
	FT_STRCPY_EXPECT("yope\0la");
	FT_STRCPY_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.");
	FT_STRCPY_EXPECT("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0b\x0c\x0d\x0e\x0f");
	FT_STRCPY_EXPECT("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0b\x0c\x0d\x0e\x0f");
    return (failure != NULL);
}

void ft_strcpy_test() {tester("ft_strcpy", &ft_strcpy_test_list, &ft_strcpy_error);};