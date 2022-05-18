#include "test.h"

/* function a tester*/
int		ft_read(int fd, char *s, int size);



struct test_struct {
    int                     fd;
    char                    *data;
    int                     len;
    int                     expected;
    int                     expected_errno;
    int                     answer_errno;
    int                     answer;
    int                     segfault;
    struct test_struct      *next;
    char                    realbuff[10000];
    char                    readbuff[10000];
};
static struct test_struct *failure = NULL;

static int TEST_SEGFAULT(int fd, char *s, int size) {
    int pid = 0;

    if ((pid = fork()) < 0)
		exit(EXIT_FAILURE);
	if (pid == 0) {
        ft_read(fd , s, size);
		exit(EXIT_SUCCESS);
    }
    wait(&pid);
    return (WIFSIGNALED(pid));
}

static int counter = -1;
static void FT_READ_EXPECT(int filed, char *str, int size) {
    int     fd[2];
    counter++;
    struct test_struct *test;
    if (counter % 10 == 0 && counter)
        dprintf(1,"\n\t\t\t");

    test = malloc(sizeof(struct test_struct));
    bzero(test, sizeof(struct test_struct));

	if (pipe(fd) < 0) exit(EXIT_FAILURE);
	fcntl(fd[0], F_SETFL, O_NONBLOCK);

    if (filed) {
        close(fd[0]);
        close(fd[1]);
        fd[0] = filed;
        fd[1] = -5;
    }

    test->data = str;
    test->fd = fd[1];
    test->len = size;
    errno = 0;
    bzero(test->realbuff, sizeof(test->realbuff));
	write(fd[1], str, size);
    test->expected_errno = errno;
	test->expected = read(fd[0], test->realbuff, size);
	write(fd[1], str, size);
    char buffseg[1000];
    test->segfault = TEST_SEGFAULT(fd[0], buffseg, size);
    test->next = NULL;
   
    if (!test->segfault) {
        write(fd[1], str, size);
        errno = 0;
        bzero(test->readbuff, sizeof(test->readbuff));
        test->answer = ft_read(fd[0], test->readbuff, size);
        test->answer_errno = errno;
    }
    if (!filed) {
        close(fd[0]);
        close(fd[1]);
    }

    test->segfault ? SEGFAULT : test->answer != test->expected || test->answer_errno != test->expected_errno || strncmp(test->realbuff, test->readbuff, size)? KO : OK;
    if (test->segfault || test->answer != test->expected || test->answer_errno != test->expected_errno || strncmp(test->realbuff, test->readbuff, size)) {
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


static void ft_read_error() {
   struct test_struct *tmp = failure;
    while (tmp) {
        dprintf(detail_fd, "\n\n\t%s:\033[37;3m TEST:       ft_read(%d,\"%s\", %d)\033[0m\n", tmp->segfault ? "\033[31;1msegfault" : "\033[33;3mfailure", tmp->fd, str_escape(tmp->data), tmp->len);
        char *data = str_escape(tmp->readbuff);
        dprintf(detail_fd, "\t\033[0;33m        \033[31;3m ANSWER:     %-5d ERRNO: %-5d  DATA: %s\033[0m\n", tmp->answer, tmp->answer_errno, data);
        free(data);
        data = str_escape(tmp->realbuff);
        dprintf(detail_fd, "\t\033[0;33m        \033[32;3m EXPECTED:   %-5d ERRNO: %-5d  DATA: %s\033[0m\n", tmp->expected, tmp->expected_errno, data);
        free(data);
        free(tmp);
        tmp = tmp->next;
    }
}

static int ft_read_test_list() {
    FT_READ_EXPECT(0, "qdsztzv\0", -5);
    FT_READ_EXPECT(0, "adsaddc", 3);
    FT_READ_EXPECT(0, "azerty", 5);
    FT_READ_EXPECT(95, "azerty", 5);
    FT_READ_EXPECT(123193, "azerty", 5);
    FT_READ_EXPECT(95, "azdafaeera1418 \tvzzFfj\0", 5);
    FT_READ_EXPECT(0, "\0\0\0\0\0\0\0", 5);
    FT_READ_EXPECT(1293532, NULL, 5);
    FT_READ_EXPECT(17, "azerty", 5);
    FT_READ_EXPECT(-5, "azerty", 5);
    FT_READ_EXPECT(-51319513, "azerty", 5);
    FT_READ_EXPECT(0, NULL, -5);
    FT_READ_EXPECT(0, "\0", 1);
    FT_READ_EXPECT(0, "\033[31;1m colorized text", 30);
    FT_READ_EXPECT(0, " tescwxgzeqcq<f hezqq<kcfhtext", 2148);
    FT_READ_EXPECT(0, "", 2148);
    FT_READ_EXPECT(0, "\n\t\n\v\f   \0", 6);
    FT_READ_EXPECT(0, "\n\t\n\v\fazeaze   \0csdfqzf012385qsvf", 15);
    FT_READ_EXPECT(0, "Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.", 50);
    FT_READ_EXPECT(0, "Lorem ipsum dolor sit amet, consectetur adipiscing\
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
tortor, sit amet consequat amet.", 500);
    return (failure != NULL);
}

void ft_read_test() {tester("ft_read", &ft_read_test_list, &ft_read_error);};