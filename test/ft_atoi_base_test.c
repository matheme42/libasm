#include "test.h"

/* function a tester*/
int		ft_atoi_base(const char *s, const char *base);

int atoi_base(char *str, char *base)
{
	long 	 	nb;
	char		sign;
	int			radix;

	radix = strlen(base);
	sign = 1;
	nb = 0;

	if (radix < 2)
		return (-1);

	for (int i = 0; i < radix ; i++) {
		for (int j = i + 1; j < radix ; j++) {
			if (base[i] == base[j])
				return (-1);
			if (base[i] == ' ' || (base[i] >= 9 && base[i] <= 13))
				return (-1);
		}
	}

	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;

	if (*str == '-' && radix == 10) {
		str++;
		sign = -1;
	} else if (*str == '+') {
		str++;
	}
	int i;
	while (*str) {
		i = 0;
		while (*str != base[i] && i < radix)
			i++;
		if (i == radix)
			return (-1);
		nb = nb * radix + i;

		if (nb > 2147483647 || nb < -2147483648)
			return (-1);
		str++;
	}
	return nb * sign;
}


struct test_struct {
    char                    *test;
    char                    *base;
    int                     expected;
    int                     answer;
    int                     segfault;
    struct test_struct    *next;
};
static struct test_struct *failure = NULL;

static int TEST_SEGFAULT(char *s, char *b) {
    int pid = 0;

    if ((pid = fork()) < 0)
		exit(EXIT_FAILURE);
	if (pid == 0) {
        ft_atoi_base(s, b);
		exit(EXIT_SUCCESS);
    }
    wait(&pid);
    return (WIFSIGNALED(pid));
}

static int counter = -1;
static void FT_ATOI_BASE_EXPECT(char *str, char *base) {
    counter++;
    struct test_struct *test;
    if (counter % 10 == 0 && counter)
        dprintf(1,"\n\t\t\t");

    test = malloc(sizeof(struct test_struct));
    test->expected = atoi_base(str, base);
    test->answer = 0;
    test->test = str;
    test->base = base;
    test->next = NULL;
    test->segfault = TEST_SEGFAULT(str, base);
    if (!test->segfault)
        test->answer = ft_atoi_base(str, base);
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


static void ft_atoi_base_error() {
    struct test_struct *tmp = failure;
    struct test_struct *next;

    while (tmp) {
        next = tmp->next;
        dprintf(detail_fd, "\n\n\t%s:\033[37;3m TEST:       ft_atoi_base(\"%s\", \"%s\")\033[0m\n", tmp->segfault ? "\033[31;1msegfault" : "\033[33;3mfailure", str_escape(tmp->test), str_escape(tmp->base));
        dprintf(detail_fd, "\t\033[0;33m        \033[31;3m ANSWER:     %-5d\033[0m\n", tmp->answer);
        dprintf(detail_fd, "\t\033[0;33m        \033[32;3m EXPECTED:   %-5d\033[0m\n", tmp->expected);
        free(tmp);
		tmp = next;
    }
}

static int ft_atoi_base_test_list() {
	FT_ATOI_BASE_EXPECT("0", "0987654321");
	FT_ATOI_BASE_EXPECT("12345", "0987654321");
	FT_ATOI_BASE_EXPECT("-2319415", "0123456789");
	FT_ATOI_BASE_EXPECT("0111010101001010101010001", "01");
	FT_ATOI_BASE_EXPECT("123", "0123456789");
	FT_ATOI_BASE_EXPECT("101", "01");
	FT_ATOI_BASE_EXPECT("ffe0", "0123456789abcdef");
	FT_ATOI_BASE_EXPECT("\t\n\r\v\f\r 4", "01234");
	FT_ATOI_BASE_EXPECT("asdz", "abasdza");
	FT_ATOI_BASE_EXPECT("ityeza", "azertyuiop");
	FT_ATOI_BASE_EXPECT("01010000010101", "01");
	FT_ATOI_BASE_EXPECT("63214", "0123456");
	FT_ATOI_BASE_EXPECT(" -213945", "0123456789");
	FT_ATOI_BASE_EXPECT("  -0194012", "0123456789");
	FT_ATOI_BASE_EXPECT("111000", "01");
	FT_ATOI_BASE_EXPECT("ff", "0123456789abcdef");
	FT_ATOI_BASE_EXPECT("52", "01234567");
	FT_ATOI_BASE_EXPECT("2a", "0123456789abcdef");
	FT_ATOI_BASE_EXPECT("7fffffff", "0123456789abcdef");
	FT_ATOI_BASE_EXPECT("ff", "0123456789abcdef");
	FT_ATOI_BASE_EXPECT("ff", "0123456789abcdef");
	FT_ATOI_BASE_EXPECT("abcbcacbacbab", "abcc");
	FT_ATOI_BASE_EXPECT("fs", "fsqcaz");
	FT_ATOI_BASE_EXPECT("aa662231a", "a1632");
	FT_ATOI_BASE_EXPECT("000003", "0123");
	FT_ATOI_BASE_EXPECT("0110101010101011011", "01");
	FT_ATOI_BASE_EXPECT("ff", "0123456789abcdef");
	FT_ATOI_BASE_EXPECT("79435ff", "0123456789abcdef");
	FT_ATOI_BASE_EXPECT("195843affff", "0123456789abf");
	FT_ATOI_BASE_EXPECT("+bcdffff", "012345bcdef");
	FT_ATOI_BASE_EXPECT("-01", "01");
	FT_ATOI_BASE_EXPECT("--11101", "01");
	FT_ATOI_BASE_EXPECT("+-123", "01");
	FT_ATOI_BASE_EXPECT("-++++01++-+--ff", "01");
	FT_ATOI_BASE_EXPECT("-++10101\xff", "01");
	FT_ATOI_BASE_EXPECT("-+\r++-+--01\x01", "01");
	FT_ATOI_BASE_EXPECT("a\0bb", "ab");
	FT_ATOI_BASE_EXPECT("-b\0bb", "ab");
	FT_ATOI_BASE_EXPECT(" \t\n\r-++++++-+--ff\xff", "0123456789abcdef");
	FT_ATOI_BASE_EXPECT("  -+\r++-+--ff\xff", "0123456789abcdef");
	FT_ATOI_BASE_EXPECT("\n-01", "01");
	FT_ATOI_BASE_EXPECT("\x0b--11101", "01");
	FT_ATOI_BASE_EXPECT(" 755x+", "01234567");
	FT_ATOI_BASE_EXPECT(" 700chmod", "01234567");
	FT_ATOI_BASE_EXPECT(" 644yo", "01234567");
	FT_ATOI_BASE_EXPECT(" 755", "01234567");
	FT_ATOI_BASE_EXPECT(" 7777777", "01234567");
	FT_ATOI_BASE_EXPECT(" 000000000000", "01234567");
	FT_ATOI_BASE_EXPECT(" 2147483647", "0123456789");
	FT_ATOI_BASE_EXPECT(" -2147483647", "0123456789");
    return (failure != NULL);
}

void ft_atoi_base_test() {tester("ft_atoi_base", &ft_atoi_base_test_list, &ft_atoi_base_error);};