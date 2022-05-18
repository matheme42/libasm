#include "test.h"

/* function a tester*/
void	ft_putnbr(long nb);

void    putnbr(long nb) {
    char buff[21];
    int  idx = 0;
    int neg = 0;
    if (nb < 0) {
        neg = 1;
    } else if (nb == 0){
         buff[20 - idx++] = '0';
    } else{
        nb = -nb;
    }

    while (nb) {
        buff[20 - idx++] = (-nb % 10) + '0';
        nb = nb / 10;
    }
    if (neg == 1)
        buff[20 - idx++] = '-'; 
   write(1, &buff[20 - idx + 1], idx);
}

struct test_struct {
    long                    test;
    char                    expected[21];
    char                    answer[21];
    int                     segfault;
    struct test_struct      *next;
};
static struct test_struct *failure = NULL;

static int TEST_SEGFAULT(long nb) {
    int pid = 0;

    if ((pid = fork()) < 0)
		exit(EXIT_FAILURE);
	if (pid == 0) {
        ft_putnbr(nb);
		exit(EXIT_SUCCESS);
    }
    wait(&pid);
    return (WIFSIGNALED(pid));
}

static int counter = -1;
static void FT_PUTNBR_EXPECT(long nb) {
    counter++;
    int fd[2];
    char buff[21];
    struct test_struct *test;
    if (counter % 10 == 0 && counter)
        dprintf(1,"\n\t\t\t");

    test = malloc(sizeof(struct test_struct));

	if (pipe(fd) < 0) exit(EXIT_FAILURE);
	fcntl(fd[0], F_SETFL, O_NONBLOCK);

    int stdout_fd = dup(1);
    dup2(fd[1], 1);
    
    putnbr(nb);
    bzero(test->expected, 21);
    read(fd[0], test->expected, 21);
    test->test = nb;
    test->next = NULL;
    test->segfault = TEST_SEGFAULT(nb);
    bzero(buff, 21);
    read(fd[0], buff, 21);
    if (!test->segfault) {
        ft_putnbr(nb);
        bzero(test->answer, 21);
        read(fd[0], test->answer, 21);
    }
    dup2(stdout_fd, 1);
    close(fd[0]);
    close(fd[1]);

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


static void ft_putnbr_error() {
    struct test_struct *tmp = failure;
    struct test_struct *next;

    while (tmp) {
        next = tmp->next;
        dprintf(detail_fd, "\n\n\t%s:\033[37;3m TEST:       ft_putnbr(\"%ld\")\033[0m\n", tmp->segfault ? "\033[31;1msegfault" : "\033[33;3mfailure", tmp->test);
        dprintf(detail_fd, "\t\033[0;33m        \033[31;3m ANSWER:     %s\033[0m\n", tmp->answer);
        dprintf(detail_fd, "\t\033[0;33m        \033[32;3m EXPECTED:   %s\033[0m\n", tmp->expected);
        free(tmp);
        tmp = next;
    }
}

static int ft_putnbr_test_list() {
    FT_PUTNBR_EXPECT(0);
    FT_PUTNBR_EXPECT(-1);
    FT_PUTNBR_EXPECT(-13941);
    FT_PUTNBR_EXPECT(-612496245);
    FT_PUTNBR_EXPECT(123456);
    FT_PUTNBR_EXPECT(6);
    FT_PUTNBR_EXPECT(-6);
    FT_PUTNBR_EXPECT(12394);
    FT_PUTNBR_EXPECT(-543210);
    FT_PUTNBR_EXPECT(999999999999);
    FT_PUTNBR_EXPECT(-999999999999);
    FT_PUTNBR_EXPECT(0x12394);
    FT_PUTNBR_EXPECT(419634241123123);
    FT_PUTNBR_EXPECT(0XFFFFFFFF);
    FT_PUTNBR_EXPECT(66666676);
    FT_PUTNBR_EXPECT(-2147483658);
    FT_PUTNBR_EXPECT(2147483657);
    FT_PUTNBR_EXPECT(0XFFFFFFFFFF);
    FT_PUTNBR_EXPECT(9223372036854775807);
    FT_PUTNBR_EXPECT(-9223372036854775807);
    return (failure != NULL);
}

void ft_putnbr_test() {tester("ft_putnbr", &ft_putnbr_test_list, &ft_putnbr_error);};