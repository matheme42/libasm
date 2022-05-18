# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: t <t@student.42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/10/03 11:06:26 by yalabidi          #+#    #+#              #
#    Updated: 2022/05/18 12:44:33 by t                ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DISTRIB_NAME = $(shell uname -s)

BLUE=\033[0;38;5;123m
LIGHT_PINK = \033[0;38;5;200m
PINK = \033[0;38;5;198m
DARK_BLUE = \033[1;38;5;110m
GREEN = \033[1;32;111m
LIGHT_GREEN = \033[1;38;5;121m
LIGHT_RED = \033[0;38;5;110m
FLASH_GREEN = \033[33;32m
WHITE_BOLD = \033[37m

# nom de l'executable
NAME= 	libasm.a
TESTER=	.tester

#sources path
SRC_PATH= srcs
SRC_TEST_PATH= test


#objects path
OBJ_PATH= .objs
OBJ_TEST_PATH= .test_objs


INCLUDE = srcs

NAME_TEST_SRC=	ft_strcpy_test.c ft_strcmp_test.c ft_strdup_test.c ft_strlen_test.c \
				main.c helper_test.c ft_write_test.c ft_read_test.c ft_atoi_base_test.c\
				ft_list_size_test.c list_helper_test.c ft_list_push_front.c ft_list_sort_test.c\
				ft_list_remove_if_test.c ft_bzero_test.c ft_putnbr_test.c\

NAME_SRC=	ft_strlen.s ft_strcmp.s ft_strcpy.s ft_read.s ft_write.s ft_strdup.s\
			ft_atoi_base.s ft_list_size.s ft_list_push_front.s ft_list_remove_if.s\
			ft_list_sort.s ft_bzero.s ft_putnbr.s


NAME_SRC_LEN	= $(shell echo -n $(NAME_SRC) | wc -w)
I				= 0
NAME_TEST_SRC_LEN	= $(shell echo -n $(NAME_SRC) | wc -w)
J				= 0


OBJ_NAME		= $(NAME_SRC:.s=.o)
OBJ_TEST_NAME	= $(NAME_TEST_SRC:.c=.o)


OBJS = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))
OBJS_TEST = $(addprefix $(OBJ_TEST_PATH)/,$(OBJ_TEST_NAME))


CC			= clang
NASM_ELF	= nasm -f elf64 -i${INCLUDE} -DSYS_WRITE=0x01 -DSYS_READ=0x00 -DERRNO=__errno_location -DMALLOC=malloc -DFREE=free
NASM_MACHO	= nasm -f macho64 -i${INCLUDE} -DSYS_WRITE=0x2000004  -DSYS_READ=0x2000003 -DERRNO=___error -DMALLOC=_malloc -DFREE=_free

ARCH		= ar rcs
CFLAGS		= -Wall -Werror -Wextra


all: $(OBJS) $(NAME)

$(NAME) : $(OBJS)
	@$(ARCH) $@  $^
	@echo "	\033[2K\r$(DARK_BLUE)$(NAME):\t\t$(GREEN)loaded\033[0m"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.s
	@mkdir $(OBJ_PATH) 2> /dev/null || true
ifeq ("$(DISTRIB_NAME)", "Linux")
	@$(NASM_ELF) $< -o $@
else
	@$(NASM_MACHO) $< -o $@
endif
	@$(eval I=$(shell echo $$(($(I)+1))))
	@printf "\033[2K\r${G}$(DARK_BLUE)>>\t\t\t\t$(I)/$(shell echo $(NAME_SRC_LEN)) ${N}$(BLUE)$<\033[36m \033[0m"




clean:
	@rm -f $(OBJS)
ifeq ("$(wildcard $(OBJ_PATH))", "")
else
	@rm -f $(OBJS)
	@rmdir $(OBJ_PATH) 2> /dev/null || true
	@printf "\033[2K\r$(DARK_BLUE)$(NAME) objects:\t$(LIGHT_PINK)removing\033[36m \033[0m\n"
endif
ifeq ("$(wildcard $(OBJ_TEST_PATH))", "")
else
	@rm -f $(OBJS_TEST)
	@rmdir $(OBJ_TEST_PATH) 2> /dev/null || true
	@printf "\033[2K\r$(DARK_BLUE)$(TESTER) objects:\t$(LIGHT_PINK)removing\033[36m \033[0m\n"
endif


fclean: clean
	@rm -f $(NAME)
ifeq ("$(wildcard $(NAME))", "")
else
	@rm -f $(NAME)
	@printf "\033[2K\r$(DARK_BLUE)$(NAME):\t\t$(PINK)removing\033[36m \033[0m\n"
endif
ifeq ("$(wildcard $(TESTER))", "")
else
	@rm -f $(TESTER)
	@printf "\033[2K\r$(DARK_BLUE)$(TESTER):\t\t$(PINK)removing\033[36m \033[0m\n"
endif

re: fclean all

test: all $(TESTER)
	@./$(TESTER) $(F)

$(TESTER) : $(OBJS) $(OBJS_TEST)
	@echo "	\033[2K\r$(DARK_BLUE)$(TESTER):\t\t$(GREEN)loaded\033[0m"
	@$(CC) $^ $(NAME) -o $@


$(OBJ_TEST_PATH)/%.o: $(SRC_TEST_PATH)/%.c
	@mkdir $(OBJ_TEST_PATH) 2> /dev/null || true
	@$(CC) -c $< -o $@
	@$(eval J=$(shell echo $$(($(J)+1))))
	@printf "\033[2K\r${G}$(DARK_BLUE)>>\t\t\t$(J)/$(shell echo $(NAME_SRC_TEST_LEN)) ${N}$(BLUE)$<\033[36m \033[0m"


.PHONY: all re clean fclean lib test silent
