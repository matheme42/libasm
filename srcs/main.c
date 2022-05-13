#include <stdio.h>		// printf
#include <string.h>		// strcpy strcmp strdup strlen
#include <unistd.h>		// read write
#include <sys/errno.h>	// errno
#include <stdlib.h>		// free
#include <fcntl.h>		// open
#include <sys/types.h>
#include <sys/wait.h>


int		ft_strlen(char *s);
int		ft_strcmp(char *s1, char* s2);
char	*ft_strcpy(char *dst, char* src);
int		ft_write(int std, char *data, int len);
int		ft_read(int fd, char *buff, int len);
char	*ft_strdup(char *s);

/* ********************************************************* */
/* *********************** FT_READ_TEST ******************** */
/* ********************************************************* */

#define FT_READ_BUF_SIZE (1 << 12)

#  define OPEN_MAX FOPEN_MAX


/*
 * segfault tester
 */
int  pid;
int signaled;
int signaled_suite;


# define TEST_SEGFAULT(x) do {               \
	if ((pid = fork()) < 0)                  \
		exit(EXIT_FAILURE);                  \
	if (pid == 0) {                          \
		do { x; } while(0);                  \
		exit(EXIT_SUCCESS);                  \
	} else {                                 \
		wait(&pid);                          \
		signaled = WIFSIGNALED(pid);         \
		if (signaled) signaled_suite = 1;    \
	}                                        \
} while(0)

char *test_name;

# define TEST_ASM_FUNCTION(x) do {       \
	TEST_SEGFAULT(x);                    \
	if (signaled)                        \
		print_signaled_ko(#x);           \
	else                                 \
		print_ok();                      \
} while(0)

# define ERRNO_WRAP(x, errno_save) do {  \
	errno = 0;                           \
	do { x; } while (0);                 \
	errno_save = errno;                  \
} while(0)

void print_ok(void)
{
	printf("\033[0;32mOK:\033[0m \033[0;34m%s\033[0m\n", test_name);
	fflush(stdout);
}

void print_signaled_ko(const char *code)
{
	printf("\033[0;31mKO: [SEGFAULT]:\033[0m \033[0;34m%s:\033[0m with: %s\n", test_name, code);
	fflush(stdout);
}

static int ft_read_pipe[2];
static char buf[FT_READ_BUF_SIZE] = {0};
static ssize_t read_ret;
static ssize_t read_origin_ret;
static int read_errno;
static int read_origin_errno;

#define FT_READ_EXPECT(str) do {                                           \
	if (pipe(ft_read_pipe) < 0)                                            \
		exit(EXIT_FAILURE);                                                \
	fcntl(ft_read_pipe[0], F_SETFL, O_NONBLOCK);                           \
	write(ft_read_pipe[1], str, strlen(str));                              \
	ERRNO_WRAP(read_origin_ret = read(ft_read_pipe[0], buf, strlen(str)), read_origin_errno);             \
	write(ft_read_pipe[1], str, strlen(str));                              \
	ERRNO_WRAP(read_ret = ft_read(ft_read_pipe[0], buf, strlen(str)), read_errno);                 \
	buf[read_ret] = '\0';                                                  \
	if (read_errno != read_origin_errno)                                   \
		printf("\033[0;31mKO: [COMPARE]:\033[0;34m %s:\033[0m \033[0;35mexpected: errno %d got: errno %d\033[0;m with: "#str"\n", \
				test_name, read_origin_errno, read_errno);                           \
	else if (strcmp(buf, str) != 0 || read_ret != read_origin_ret)                       \
		printf("\033[0;31mKO: [COMPARE]:\033[0;34m %s:\033[0;m \033[0;35mexpected: %lu got: %lu\033[0;m \"%s\" with: %d, \"%s\", %zu \n", \
				test_name, strlen(str), read_ret, buf, ft_read_pipe[0], buf, read_origin_ret);        \
	else                                                                   \
		print_ok();                                                        \
	close(ft_read_pipe[1]);                                                \
	close(ft_read_pipe[0]);                                                \
} while (0);

#define FT_READ_EXPECT_ERROR(fd, buf, size) do {                                 \
	ERRNO_WRAP(read_ret = ft_read(fd, buf, size), read_errno);                   \
	ERRNO_WRAP(read_origin_ret = read(fd, buf, size), read_origin_errno);        \
	if ((long)read_ret != -1)                                                    \
		printf("\033[0;31mKO: [COMPARE]: \033[0;34m%s:\033[0;m \033[0;35mexpected: %-2ld got: %-2ld\033[0;m  with: %-6d "#buf" %d\n", \
				test_name, -1l, (long)read_ret, fd, size);                       \
	else if (read_errno != read_origin_errno)                                    \
		printf("\033[0;31mKO: [COMPARE]: \033[0;34m%s:\033[0;m \033[0;35mexpected: errno %-2d got: errno %-2d\033[0;m  with: %-6d "#buf", %-4d\n", \
				test_name, read_origin_errno, read_errno, fd, size);                        \
	else                                                                         \
		print_ok();                                                              \
} while (0);

static
void ft_read_test_segfault(void)
{
	int tmp[2];
	char buf_read[2048];

	if (pipe(tmp) < 0)
		exit(EXIT_FAILURE);
	write(tmp[1], "asdfkasdfkl;jasd;ljkfa;lssdlfasdfasdfasdfasdfasdfasdfasdfasdfasdf", 40);
	TEST_ASM_FUNCTION(ft_read(-1, buf_read, 2));
	TEST_ASM_FUNCTION(ft_read(tmp[0], NULL, 2));
	TEST_ASM_FUNCTION(ft_read(tmp[0], buf_read, 0));
	TEST_ASM_FUNCTION(ft_read(tmp[0], buf_read, 5));
	TEST_ASM_FUNCTION(ft_read(tmp[0], buf_read, 32));
	close(tmp[0]);
	close(tmp[1]);
	TEST_ASM_FUNCTION(ft_read(-1, buf_read, 2));
	TEST_ASM_FUNCTION(ft_read(OPEN_MAX + 1, buf_read, 2));
}

static
void ft_read_test_compare(void)
{
	FT_READ_EXPECT("");
	FT_READ_EXPECT("bon");
	FT_READ_EXPECT("bonjour");
	FT_READ_EXPECT("#c#s#p#x#X#e#f#g");
	FT_READ_EXPECT("the\0hidden");
	FT_READ_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.");

	int tmp[2];
	pipe(tmp);
	char buf[32];

	write(tmp[1], "bonjour", 7);
	FT_READ_EXPECT_ERROR(tmp[0], NULL, 3);
	FT_READ_EXPECT_ERROR(-1, buf, 7);
	FT_READ_EXPECT_ERROR(42, buf, 7);
	FT_READ_EXPECT_ERROR(9809, buf, 7);
	FT_READ_EXPECT_ERROR(98123, buf, 1);
	FT_READ_EXPECT_ERROR(42, NULL, 7);
}

void ft_read_test(void)
{
	test_name = "ft_read.s";
	signaled_suite = 0;
	ft_read_test_segfault();
	if (!signaled_suite)
		ft_read_test_compare();
}


/* ********************************************************* */
/* *********************** FT_WRITE_TEST ******************* */
/* ********************************************************* */



#define FT_WRITE_BUF_SIZE (1 << 12)

static int ft_write_pipe[2];
static char buf2[FT_WRITE_BUF_SIZE] = {0};
static ssize_t write_ret;
static ssize_t write_origin_ret;
static int write_errno;
static int write_origin_errno;
static ssize_t ret;

#define FT_WRITE_EXPECT(str) do {                                                                  \
	if (pipe(ft_write_pipe) < 0)                                                                   \
		exit(EXIT_FAILURE);                                                                        \
	fcntl(ft_write_pipe[0], F_SETFL, O_NONBLOCK);                                                  \
	ERRNO_WRAP(write_origin_ret = write(ft_write_pipe[1], str, strlen(str)), write_origin_errno);  \
	read(ft_write_pipe[0], buf2, FT_WRITE_BUF_SIZE);                                                \
	ERRNO_WRAP(write_ret = ft_write(ft_write_pipe[1], str, strlen(str)), write_errno);             \
	ret = read(ft_write_pipe[0], buf2, FT_WRITE_BUF_SIZE);                                          \
	buf2[ret] = '\0';                                                                               \
	if (write_errno != write_origin_errno)                                                         \
		printf("\033[0;31mKO: [COMPARE]: \033[0;34m%s: \033[0;35mexpected: errno %d got: errno \033[0m%d with: "#str"\n",               \
				test_name, write_origin_errno, write_errno);                                       \
	else if (strcmp(buf2, str) != 0 || write_ret != write_origin_ret)                               \
		printf("\033[0;31mKO: [COMPARE]: \033[0;34m%s: \033[0;35mexpected: %lu got: %lu \033[0m\"%s\" with: %d, \"%s\", %zu \n", \
				test_name, strlen(str), write_ret, buf2, ft_write_pipe[0], buf2, strlen(str));  \
	else                                                                                           \
		print_ok();                                                                                \
	close(ft_write_pipe[1]);                                                                       \
	close(ft_write_pipe[0]);                                                                       \
} while (0);

#define FT_WRITE_EXPECT_ERROR(fd, str, size) do {                                           \
	ERRNO_WRAP(write_ret = ft_write(fd, str, size), write_errno);                           \
	ERRNO_WRAP(write_origin_ret = write(fd, str, size), write_origin_errno);                \
	if ((long)write_ret != -1)                                                              \
		printf("\033[0;31mKO: [COMPARE]: \033[0;34m%s: \033[0;35mexpected: %ld got: %ld \033[0mwith: %d "#str", %d\n",           \
				test_name, -1l, (long)write_ret, fd, size);                                 \
	else if (write_errno != write_origin_errno)                                             \
		printf("\033[0;31mKO: [COMPARE]: \033[0;34m%s: \033[0;35mexpected: errno %d got: errno %d \033[0mwith: %d "#str", %d\n", \
				test_name, write_origin_errno, write_errno, fd, size);                      \
	else                                                                                    \
		print_ok();                                                                         \
} while (0);

static
void ft_write_test_segfault(void)
{
	int tmp[2];
	if (pipe(tmp) < 0)
		exit(EXIT_FAILURE);
	TEST_ASM_FUNCTION(ft_write(-1, "test", 5));
	TEST_ASM_FUNCTION(ft_write(tmp[1], NULL, 5));
	TEST_ASM_FUNCTION(ft_write(tmp[1], "test", 0));
	TEST_ASM_FUNCTION(ft_write(tmp[1], "test", 5));
	TEST_ASM_FUNCTION(ft_write(tmp[1], "t", 1));
	TEST_ASM_FUNCTION(ft_write(tmp[1], "", 0));
	TEST_ASM_FUNCTION(ft_write(tmp[1], "test", 4));
	TEST_ASM_FUNCTION(ft_write(tmp[1], "test", 2));
	TEST_ASM_FUNCTION(ft_write(tmp[1], NULL, 2));
	close(tmp[0]);
	close(tmp[1]);
	TEST_ASM_FUNCTION(ft_write(-1, "tt", 2));
	TEST_ASM_FUNCTION(ft_write(OPEN_MAX + 1, "tt", 2));
}

static
void ft_write_test_compare(void)
{
	FT_WRITE_EXPECT("");
	FT_WRITE_EXPECT("bon");
	FT_WRITE_EXPECT("bonjour");
	FT_WRITE_EXPECT("#c#s#p#x#X#e#f#g");
	FT_WRITE_EXPECT("the\0hidden");
	FT_WRITE_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.");

	FT_WRITE_EXPECT_ERROR(STDOUT_FILENO, NULL, 3);
	FT_WRITE_EXPECT_ERROR(-1, "bonjour", 7);
	FT_WRITE_EXPECT_ERROR(42, "bonjour", 7);
	FT_WRITE_EXPECT_ERROR(9809, "bonjour", 7);
	FT_WRITE_EXPECT_ERROR(98123, "", 1);
	FT_WRITE_EXPECT_ERROR(42, NULL, 7);
}

void ft_write_test(void)
{
	test_name = "ft_write.s";
	signaled_suite = 0;
	ft_write_test_segfault();
	if (!signaled_suite)
		ft_write_test_compare();
}

/* ********************************************************* */
/* ********************** FT_STRCPY_TEST ******************* */
/* ********************************************************* */

#define FT_STRCPY_BUF_SIZE (1 << 12)

static char expected_buf[FT_STRCPY_BUF_SIZE] = {0};
static char actual_buf[FT_STRCPY_BUF_SIZE] = {0};
static char pre_expected_buf[FT_STRCPY_BUF_SIZE] = {0};
static char pre_actual_buf[FT_STRCPY_BUF_SIZE] = {0};

#define FT_STRCPY_EXPECT(str) do {             \
	strcpy(pre_expected_buf, expected_buf);    \
	strcpy(pre_actual_buf, actual_buf);        \
	strcpy(expected_buf, str);                 \
	ft_strcpy(actual_buf, str);                \
	if (strcmp(expected_buf, actual_buf) != 0) \
		printf("\033[0;31mKO: [COMPARE]: \033[0;34m%s: \033[0;35mexpected: \"%s\" got: \"%s\" \033[0mwith: \"%s\", \"%s\"\n",\
				test_name, expected_buf, actual_buf, pre_expected_buf, pre_actual_buf); \
	else                                       \
		print_ok();                            \
} while (0);

static
void ft_strcpy_test_segfault(void)
{
	char buf[FT_STRCPY_BUF_SIZE] = {0};

	TEST_ASM_FUNCTION(ft_strcpy(buf, ""));
	TEST_ASM_FUNCTION(ft_strcpy(buf, "abc"));
	TEST_ASM_FUNCTION(ft_strcpy(buf, "asl;fjl;asdjfjkasdl;fjadjsf"));
	TEST_ASM_FUNCTION(ft_strcpy(buf, "yope\0la"));
	TEST_ASM_FUNCTION(ft_strcpy(buf, "Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet."));
}

static
void ft_strcpy_test_compare(void)
{
	FT_STRCPY_EXPECT("");
	FT_STRCPY_EXPECT("abc");
	FT_STRCPY_EXPECT("asl;fjl;asdjfjkasdl;fjadjsf");
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
}

void ft_strcpy_test(void)
{
	test_name = "ft_strcpy.s";
	signaled_suite = 0;
	ft_strcpy_test_segfault();
	if (!signaled_suite)
		ft_strcpy_test_compare();
}

/* ********************************************************* */
/* ********************** FT_STRCMP_TEST ******************* */
/* ********************************************************* */

int strcmp_expected;
int strcmp_actual;

# define FT_STRCMP_EXPECT(s1, s2) do { \
	strcmp_expected = strcmp(s1, s2); \
	strcmp_actual = ft_strcmp(s1, s2); \
	if ((strcmp_expected < 0 && strcmp_actual < 0) || \
	    (strcmp_expected > 0 && strcmp_actual > 0) || \
	    (strcmp_expected == 0 && strcmp_actual == 0)) \
		print_ok(); \
	else \
		printf("\033[0;31mKO: [COMPARE]: \033[0;34mft_strcmp.s: \033[0;35mexpected: %d got: %d \033[0mwith: "#s1", "#s2"\n", \
			   strcmp_expected, strcmp_actual); \
} while (0);

static
void ft_strcmp_test_segfault(void)
{
	TEST_ASM_FUNCTION(ft_strcmp("", ""));
	TEST_ASM_FUNCTION(ft_strcmp("bon", "bon"));
	TEST_ASM_FUNCTION(ft_strcmp("bonjour", "bonjour"));
	TEST_ASM_FUNCTION(ft_strcmp("abcdefghijasdf1324''klji//", "abcdefghijasdf1324''klji//"));
	TEST_ASM_FUNCTION(ft_strcmp("the\0hidden", "the\0hidden"));
	TEST_ASM_FUNCTION(ft_strcmp("Lorem ipsum dolor sit amet, consectetur adipiscing\
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
tortor, sit amet consequat amet."));
}

static
void ft_strcmp_test_compare(void)
{
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
}

void ft_strcmp_test(void)
{
	test_name = "ft_strcmp.s";
	signaled_suite = 0;
	ft_strcmp_test_segfault();
	if (!signaled_suite)
		ft_strcmp_test_compare();
}


/* ********************************************************* */
/* ********************** FT_STRLEN_TEST ******************* */
/* ********************************************************* */


int actual;
int expected;

#define FT_STRLEN_EXPECT(str) do {            \
	actual = ft_strlen(str);                  \
	expected = strlen(str);                   \
	if (actual != expected)                   \
		printf("\033[0;31mKO: [COMPARE]: \033[0;34m%s: \033[0;35mexpected: %d got: %d \033[0mwith: "#str"\n", \
				test_name, expected, actual); \
	else                                      \
		print_ok();                           \
} while (0);

static void ft_strlen_test_segfault(void)
{
	TEST_ASM_FUNCTION(ft_strlen(""));
	TEST_ASM_FUNCTION(ft_strlen("bon"));
	TEST_ASM_FUNCTION(ft_strlen("bonjour"));
	TEST_ASM_FUNCTION(ft_strlen("asdfasdf''///##!!@"));
	TEST_ASM_FUNCTION(ft_strlen("the\0hidden"));
	TEST_ASM_FUNCTION(ft_strlen("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet."));
}

static
void ft_strlen_test_compare(void)
{
	FT_STRLEN_EXPECT("");
	FT_STRLEN_EXPECT("bon");
	FT_STRLEN_EXPECT("bonjour");
	FT_STRLEN_EXPECT("asdfasdf''///##!!@");
	FT_STRLEN_EXPECT("the\0hidden");
	FT_STRLEN_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.");
}

void ft_strlen_test(void)
{
	test_name = "ft_strlen.s";
	signaled_suite = 0;
	ft_strlen_test_segfault();
	if (!signaled_suite)
		ft_strlen_test_compare();
}


/* ********************************************************* */
/* ********************** FT_STRDUP_TEST ******************* */
/* ********************************************************* */



static char *tmp;

#define FT_STRDUP_EXPECT(str) do {            \
	tmp = ft_strdup(str);                     \
	if (tmp == NULL || strcmp(str, tmp) != 0) \
		printf("KO: [COMPARE]: %s: expected: \"%s\" got: \"%s\"\n", test_name, str, tmp); \
	else                                      \
		print_ok();                           \
	free(tmp);                                \
} while (0);

static
void ft_strdup_test_segfault(void)
{
	char *tmp2 = NULL;

	TEST_ASM_FUNCTION(ft_strdup(""));
	TEST_ASM_FUNCTION(ft_strdup("abc"));
	TEST_ASM_FUNCTION(ft_strdup("asl;fjl;asdjfjkasdl;fjadjsf"));
	TEST_ASM_FUNCTION(ft_strdup("yope\0la"));
	TEST_ASM_FUNCTION(ft_strdup("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet."));

	tmp2 = ft_strdup("");
	TEST_ASM_FUNCTION(char a = *tmp2; a++;);
	tmp2 = ft_strdup("abc");
	TEST_ASM_FUNCTION(char a = *tmp2; a++;);
	tmp2 = ft_strdup("asl;fjl;asd");
	TEST_ASM_FUNCTION(char a = *tmp2; a++;);
	tmp2 = ft_strdup("yope\0la");
	TEST_ASM_FUNCTION(char a = *tmp2; a++;);
}

static
void ft_strdup_test_compare(void)
{
	FT_STRDUP_EXPECT("");
	FT_STRDUP_EXPECT("abc");
	FT_STRDUP_EXPECT("asl;fjl;asdjfjkasdl;fjadjsf");
	FT_STRDUP_EXPECT("yope\0la");
	FT_STRDUP_EXPECT("Lorem ipsum dolor sit amet, consectetur adipiscing\
elit. Sed in malesuada purus. Etiam a scelerisque massa. Ut non euismod elit. Aliquam\
bibendum dolor mi, id fringilla tellus pulvinar eu. Fusce vel fermentum sem. Cras\
volutpat, eros eget rhoncus rhoncus, diam augue egestas dolor, vitae rutrum nisi\
felis sed purus. Mauris magna ex, mollis non suscipit eu, lacinia ac turpis. Phasellus\
ac tortor et lectus fermentum lobortis eu at mauris. Vestibulum sit amet posuere\
tortor, sit amet consequat amet.");
	FT_STRDUP_EXPECT("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0b\x0c\x0d\x0e\x0f");
	FT_STRDUP_EXPECT("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0b\x0c\x0d\x0e\x0f");
}

void ft_strdup_test(void)
{
	test_name = "ft_strdup.s";
	signaled_suite = 0;
	ft_strdup_test_segfault();
	if (!signaled_suite)
		ft_strdup_test_compare();
}

int main() {

	ft_read_test();
	ft_write_test();
	ft_strcpy_test();
	ft_strlen_test();
	ft_strdup_test();
	ft_strcmp_test();
	return (0);
}