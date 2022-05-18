#include "test.h"

int     detail_fd = -5;
char    *test_function = NULL;

int main(int ac, char **av) {

    if (ac == 2) {
        test_function = av[1];
        detail_fd = 1;
    }

    /// Obligatoire
    if (test_function == NULL)
        dprintf(1,"\033[37;1m\t\t\t\t\tMandatory Part\n");
    ft_strlen_test();
    ft_strcmp_test();
    ft_strcpy_test();
    ft_strdup_test();
    ft_write_test();
    ft_read_test();


    // Bonus
    if (test_function == NULL)
        dprintf(1,"\n\033[33;1m\t\t\t\t\t  Bonus Part\n");
    ft_atoi_base_test();
    ft_list_size_test();
    ft_list_push_front_test();
    ft_list_sort_test();
    ft_list_remove_if_test();

    // Extra
    if (test_function == NULL)
        dprintf(1,"\n\033[35;1m\t\t\t\t\t  Extra Part\n");
    ft_bzero_test();
    ft_putnbr_test();
}